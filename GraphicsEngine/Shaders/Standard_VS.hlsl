#include "ShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
	VertexToPixel result;

	float4 vertexPosition = input.myPosition;

	if (OB_HasBones)
	{
		vertexPosition = 0;
		vertexPosition += input.myBoneWeights.x * mul(input.myPosition, OB_BoneData[input.myBoneIDs.x]);
		vertexPosition += input.myBoneWeights.y * mul(input.myPosition, OB_BoneData[input.myBoneIDs.y]);
		vertexPosition += input.myBoneWeights.z * mul(input.myPosition, OB_BoneData[input.myBoneIDs.z]);
		vertexPosition += input.myBoneWeights.w * mul(input.myPosition, OB_BoneData[input.myBoneIDs.w]);
	}

	const float4 vertexObjectPosition = vertexPosition;

	const float4 vertexWorldPosition = mul(OB_ToWorld, vertexObjectPosition);

	const float4 vertexViewPosition = mul(FB_ToView, vertexWorldPosition);

	const float4 vertexProjectionPosition = mul(FB_ToProjection, vertexViewPosition);

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