#include "CBuffers.hlsli"
#include "ShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
	VertexToPixel result;

	//float4 vertexPosition = input.myPosition;

	float4x4 skinningMatrix = 
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};

	if (OB_HasBones)
	{
		skinningMatrix = 0;

		skinningMatrix += input.myBoneWeights.x * OB_BoneData[input.myBoneIDs.x];
		skinningMatrix += input.myBoneWeights.y * OB_BoneData[input.myBoneIDs.y];
		skinningMatrix += input.myBoneWeights.z * OB_BoneData[input.myBoneIDs.z];
		skinningMatrix += input.myBoneWeights.w * OB_BoneData[input.myBoneIDs.w];

		//Old code that only modified vertex position, we need normals, tangents and binormals to also follow the vertices
		/*vertexPosition = 0;
		vertexPosition += input.myBoneWeights.x * mul(input.myPosition, OB_BoneData[input.myBoneIDs.x]);
		vertexPosition += input.myBoneWeights.y * mul(input.myPosition, OB_BoneData[input.myBoneIDs.y]);
		vertexPosition += input.myBoneWeights.z * mul(input.myPosition, OB_BoneData[input.myBoneIDs.z]);
		vertexPosition += input.myBoneWeights.w * mul(input.myPosition, OB_BoneData[input.myBoneIDs.w]);*/
	}

	//const float4 vertexObjectPosition = vertexPosition;
	//const float4 vertexWorldPosition = mul(OB_ToWorld, vertexObjectPosition);

	float4 vertexWorldPosition = mul(OB_ToWorld, mul(input.myPosition, skinningMatrix));

	if (OB_IsInstanced)
	{
		vertexWorldPosition = mul(input.World, mul(input.myPosition, skinningMatrix));
	}

	result.myVertexPosition = vertexWorldPosition;
	const float4 vertexViewPosition = mul(FB_ToView, vertexWorldPosition);
	const float4 vertexProjectionPosition = mul(FB_ToProjection, vertexViewPosition);

	// Move normals from local space to world space
	const float3x3 worldNormalRotation = (float3x3)OB_ToWorld;
	const float3x3 skinNormalRotation = (float3x3)skinningMatrix;
	result.myNormal = mul(worldNormalRotation, mul(input.myNormal, skinNormalRotation));
	result.myTangent = mul(worldNormalRotation, mul(input.myTangent, skinNormalRotation));
	result.myBinormal = mul(worldNormalRotation, mul(input.myBinormal, skinNormalRotation));

	result.myPosition = vertexProjectionPosition;

	result.myVertexColor0 = input.myVertexColor0;
	result.myVertexColor1 = input.myVertexColor1;
	result.myVertexColor2 = input.myVertexColor2;
	result.myVertexColor3 = input.myVertexColor3;

	result.myUV0 = input.myUV0;
	result.myUV1 = input.myUV1;
	result.myUV2 = input.myUV2;
	result.myUV3 = input.myUV3;

	return result;
}