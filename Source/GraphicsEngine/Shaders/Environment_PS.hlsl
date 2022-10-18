#include "CBuffers.hlsli"
#include "PBRFunctions.hlsli"
#include "ShaderStructs.hlsli"
#include "PostProcessStructs.hlsli"

#define MAX_LIGHTS 64
#include "LightBuffer.hlsli"

DeferredPixelOutput main(DeferredVertexToPixel input)
{
	DeferredPixelOutput result;
	result.Color = float4(0, 0, 0, 0);

	const float4 albedo = albedoTexture.Sample(defaultSampler, input.myUV);

	if (albedo.a <= 0.05f)
	{
		discard;
		return result;
	}

	//Sample GBuffer Texture channels
	const float3 normal = normalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 surface = surfaceTexture.Sample(defaultSampler, input.myUV);
	const float3 vertexNormal = vertexNormalTexture.Sample(defaultSampler, input.myUV).rgb;
	const float4 worldPosition = worldPositionTexture.Sample(defaultSampler, input.myUV);
	const float ambientOcclusion = ambientOcclusionTexture.Sample(defaultSampler, input.myUV).r;
	//

	//Extract values from surface texture
	const float metalness = surface.r;
	const float roughness = surface.g;
	const float emissive = surface.b;
	const float emissiveStrength = surface.a;
	//

	//Calculate emission
	#define EMISSIVE_BOOST (2.0f);
	const float3 emissiveColor = albedo.rgb * emissive * emissiveStrength * EMISSIVE_BOOST;
	//

	//Get toeye, speccolor, diffusecolor
	const float3 toEye = normalize(FB_CamTranslation.xyz - worldPosition.xyz);
	const float3 specularColor = lerp((float3)0.04f, (float3)albedo, metalness);
	const float3 diffuseColor = lerp((float3)0.00f, (float3)albedo, 1 - metalness);

	//Calc Ambient+DirLight
	float3 ambientLighting = EvaluateAmbience(environmentTexture, normal, vertexNormal, toEye, roughness, ambientOcclusion, diffuseColor, specularColor, defaultSampler);
	float3 directionalLighting = EvaluateDirectionalLight(diffuseColor, specularColor, normal, roughness, LB_DirectionalLight.Color, LB_DirectionalLight.Intensity, -LB_DirectionalLight.Direction, toEye);
	//

	//Apply SSAO
	const float ssao = TextureSlot9.Sample(defaultSampler, input.myUV).r;

	if (FB_RenderMode % 2 == 0)
	{
		ambientLighting *= ssao;
	}
	//

	//Peter panning, tune if it appears
	const float shadowBias = 0.00005f;

	//Calc DirLight Shadows
	if (LB_DirectionalLight.CastShadows)
	{
		const float4 worldToLightView = mul(LB_DirectionalLight.ViewMatrix[0], worldPosition);
		const float4 lightViewToLightProj = mul(LB_DirectionalLight.ProjectionMatrix, worldToLightView);

		float2 projectedTexCoord; //Shadow map UVS
		projectedTexCoord.x = lightViewToLightProj.x / lightViewToLightProj.w / 2.0f + 0.5f;
		projectedTexCoord.y = -lightViewToLightProj.y / lightViewToLightProj.w / 2.0f + 0.5f;

		if (saturate(projectedTexCoord.x) == projectedTexCoord.x && saturate(projectedTexCoord.y) == projectedTexCoord.y)
		{
			//Tune if shadows should be brighter
			const float shadowMult = 0.0f;

			//Rough estimate of view depth from light to point
			const float viewDepth = lightViewToLightProj.z / lightViewToLightProj.w - shadowBias;

			//Shadow map value rendered from light camera
			const float lightDepth = dirLightShadowMap.Sample(pointClampSampler, projectedTexCoord).r;

			//P < D, if depth is lower than dist to point
			if (lightDepth < viewDepth)
			{
				directionalLighting *= shadowMult;
			}
		}
	}
	//

	//Calculate Point- & Spotlights
	float3 pointLight = 0;
	float3 spotLight = 0;

	[unroll(32)]
	for (unsigned int l = 0; l < LB_NumLights; ++l)
	{
		const LightData light = LB_Lights[l];

		switch (light.LightType)
		{
			//Directional Light
			case 0:
				break;
			//Ambient light
			case 1:
				break;
			//Point Light
			case 2:
			{
				bool evaluatePointLight = false;

				if (light.CastShadows)
				{
					for (unsigned int i = 0; i < 6; ++i)
					{
						const float4 worldToLightView = mul(light.ViewMatrix[i], worldPosition);
						const float4 lightViewToLightProj = mul(light.ProjectionMatrix, worldToLightView);

						float2 projectedTexCoord; //Shadow map UVS
						projectedTexCoord.x = lightViewToLightProj.x / lightViewToLightProj.w / 2.0f + 0.5f;
						projectedTexCoord.y = -lightViewToLightProj.y / lightViewToLightProj.w / 2.0f + 0.5f;

						if (saturate(projectedTexCoord.x) == projectedTexCoord.x && saturate(projectedTexCoord.y) == projectedTexCoord.y)
						{
							//Rough estimate of view depth from light to point
							const float viewDepth = lightViewToLightProj.z / lightViewToLightProj.w - shadowBias;

							//Shadow map value rendered from light camera
							const float lightDepth = pointLightShadowMaps[i].Sample(pointClampSampler, projectedTexCoord).r;

							//P >= D, if depth is higher than dist to point
							if (lightDepth >= viewDepth)
							{
								evaluatePointLight = true;
							}
						}
					}
				}
				else
				{
					evaluatePointLight = true;
				}

				if (evaluatePointLight)
				{
					pointLight += EvaluatePointLight(diffuseColor, specularColor, normal, roughness, light.Color, light.Intensity, light.Range,
						light.Position, toEye, worldPosition.xyz);
				}

				break;
			}
			//Spot Light
			case 3:
			{
				if (light.CastShadows)
				{
					const float4 worldToLightView = mul(light.ViewMatrix[0], worldPosition);
					const float4 lightViewToLightProj = mul(light.ProjectionMatrix, worldToLightView);

					float2 projectedTexCoord; //Shadow map UVS
					projectedTexCoord.x = lightViewToLightProj.x / lightViewToLightProj.w / 2.0f + 0.5f;
					projectedTexCoord.y = -lightViewToLightProj.y / lightViewToLightProj.w / 2.0f + 0.5f;

					if (saturate(projectedTexCoord.x) == projectedTexCoord.x && saturate(projectedTexCoord.y) == projectedTexCoord.y)
					{
						//Rough estimate of view depth from light to point
						const float viewDepth = lightViewToLightProj.z / lightViewToLightProj.w - shadowBias;

						//Shadow map value rendered from light camera
						const float lightDepth = spotLightShadowMap.Sample(pointClampSampler, projectedTexCoord).r;

						//P < D, if depth is lower than dist to point
						if (lightDepth < viewDepth)
						{
							break;
						}
					}
				}

				spotLight += EvaluateSpotLight(diffuseColor, specularColor, normal, roughness, light.Color, light.Intensity, light.Range, 
					light.Position, light.Direction, light.SpotOuterRadius, light.SpotInnerRadius, toEye, worldPosition.xyz);
				break;
			}
			default:
			{
				break;
			}
		}
	}
	//

	//assemble light, no more lineartogamma due to postprocess effects
	result.Color.rgb = ambientLighting + directionalLighting + emissiveColor + pointLight + spotLight;
	result.Color.a = 1.0f;

	//TODO: Rendermodes for deferred rendering

	return result;
}