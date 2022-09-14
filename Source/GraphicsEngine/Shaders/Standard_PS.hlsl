#include "CBuffers.hlsli"
#include "PBRFunctions.hlsli"
#include "ShaderStructs.hlsli"

#define MAX_LIGHTS 8
#include "LightBuffer.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput result;

	//Single color
	//result.myColor = input.myVertexColor0;
	//result.myColor = float4(MB_Albedo, 1);

	//Solid material color with variation
	//const float average = (input.myVertexColor0.r + input.myVertexColor0.g + input.myVertexColor0.b) / 3;
	//result.myColor = average * float4(MB_Albedo, 1);

	//Texture sampling
	const float3 albedo = albedoTexture.Sample(defaultSampler, input.myUV0).rgb;
	const float3 normalMap = normalTexture.Sample(defaultSampler, input.myUV0).agb;
	const float4 surface = surfaceTexture.Sample(defaultSampler, input.myUV0);

	const float ambientOcclusion = normalMap.b;
	const float metalness = surface.r;
	const float roughness = surface.g;
	const float emissive = surface.b;
	const float emissiveStrength = surface.a;
	//

	const float3 toEye = normalize(FB_CamTranslation.xyz - input.myVertexPosition.xyz);

	//Construct TangentSpaceMatrix
	const float3x3 TangentSpaceMatrix = float3x3
	(
		normalize(input.myTangent),
		normalize(input.myBinormal),
		normalize(input.myNormal)
	);
	//

	//Extract pixelnormal from Normal Map
	float3 pixelNormal = normalMap;
	pixelNormal.z = 0;
	pixelNormal = 2.0f * pixelNormal - 1;
	pixelNormal.z = sqrt(1 - saturate(pixelNormal.x + pixelNormal.x + pixelNormal.y + pixelNormal.y));
	pixelNormal = normalize(pixelNormal);
	pixelNormal = normalize(mul(pixelNormal, TangentSpaceMatrix));
	//

	//Lambertian reflection
	//Ip=L•NCI
	// Color of lighted pixel = Direction towards light DOT Pixel normal * Light color * Light intensity
	const float3 L = -1 * normalize(LB_DirectionalLight.Direction);
	const float3 N = pixelNormal;
	const float LdotN = saturate(dot(L, N));
	const float3 C = LB_DirectionalLight.Color;
	const float I = LB_DirectionalLight.Intensity;
	const float3 finalPixelColor = LdotN * C * I;
	float3 diffuse = albedo * finalPixelColor;
	//

	//IBL: Image based lightning
	const float3 environment = environmentTexture.SampleLevel(defaultSampler, input.myNormal, 0).rgb;
	float3 ambient = albedo * environment;
	//

	//PBR
	const float3 specularColor = lerp((float3)0.04f, albedo, metalness);
	const float3 diffuseColor = lerp((float3)0.00f, albedo, 1 - metalness);
	const float3 ambientLighting = EvaluateAmbience(environmentTexture, pixelNormal, input.myNormal, toEye, roughness, ambientOcclusion, diffuseColor, specularColor, defaultSampler);
	const float3 directionalLighting = EvaluateDirectionalLight(diffuseColor, specularColor, pixelNormal, roughness, LB_DirectionalLight.Color, LB_DirectionalLight.Intensity, -LB_DirectionalLight.Direction, toEye);
	//

	float3 pointLight = 0;
	float3 spotLight = 0;

	//Calculate Point- & Spotlights
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
				pointLight += EvaluatePointLight(diffuseColor, specularColor, pixelNormal, roughness, light.Color, light.Intensity, light.Range, 
					light.Position, toEye, input.myVertexPosition.xyz);
				break;
			}
			//Spot Light
			case 3:
			{
				spotLight += EvaluateSpotLight(diffuseColor, specularColor, pixelNormal, roughness, light.Color, light.Intensity, light.Range, 
					light.Position, light.Direction, light.SpotOuterRadius, light.SpotInnerRadius, toEye, input.myVertexPosition.xyz);
				break;
			}
			default:
			{
				break;
			}
		}
	}
	//

	//PBR replace IBL temp
	ambient = ambientLighting;
	diffuse = directionalLighting;

#ifdef _DEBUG
	switch (FB_RenderMode)
	{
		case 0: //Default
			//result.myColor.rgb = saturate(ambient + diffuse);
			result.myColor.rgb = LinearToGamma(ambient + diffuse + emissive * emissiveStrength * albedo + pointLight + spotLight);
			result.myColor.a = 1;
			break;
		case 1: //UV
			result.myColor = float4(input.myUV0.r, input.myUV0.g, 0, 1);
			break;
		case 2: //VertexColor
			result.myColor = float4(input.myVertexColor0.r, input.myVertexColor0.g, input.myVertexColor0.b, input.myVertexColor0.a);
			break;
		case 3: //VertexNormal
		{
			//Normal debugging, visualize local space normals
			float3 debugNormal = input.myNormal;
			const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
			if (signedLength < 0)
			{
				debugNormal = float3(1 - abs(debugNormal));
			}
			result.myColor.rgb = debugNormal;
			result.myColor.a = 1;
			break;
		}
		case 4: //PixelNormal
		{
			//Normal debugging, visualize world space normals
			float3 debugNormal = pixelNormal;
			const float signedLength = (debugNormal.r + debugNormal.g + debugNormal.b) / 3;
			if (signedLength < 0)
			{
				debugNormal = float3(1 - abs(debugNormal));
			}
			result.myColor.rgb = debugNormal;
			result.myColor.a = 1;
			break;
		}
		case 5: //AlbedoMap
			result.myColor.rgb = albedo;
			result.myColor.a = 1;
			break;
		case 6: //NormalMap
			result.myColor = float4(normalMap.r, normalMap.g, 1, 1);
			break;
		case 7: //DiffuseLight + Albedo
			result.myColor.rgb = saturate(diffuse);
			result.myColor.a = 1;
			break;
		case 8: //DiffuseLightNoAlbedo
			result.myColor.rgb = finalPixelColor;
			result.myColor.a = 1;
			break;
		case 9: //AmbientLight + Albedo
			result.myColor.rgb = saturate(ambient);
			result.myColor.a = 1;
			break;
		case 10: //AmbientLightNoAlbedo
			result.myColor.rgb = saturate(environment);
			result.myColor.a = 1;
			break;
		case 11: //Ambient Occlusion
			result.myColor.rgb = ambientOcclusion;
			result.myColor.a = 1;
			break;
		case 12: //Roughness
			result.myColor.rgb = roughness;
			result.myColor.a = 1;
			break;
		case 13: //Metalness
			result.myColor.rgb = metalness;
			result.myColor.a = 1;
			break;
		case 14: //Emissiveness
			result.myColor.rgb = emissive * emissiveStrength;
			result.myColor.a = 1;
			break;
		case 15: //Linear
			result.myColor.rgb = ambient + diffuse;
			result.myColor.a = 1;
			break;
	}
#else
	result.myColor.rgb = LinearToGamma(ambient + diffuse);
	result.myColor.a = 1;
#endif

	return result;
}
