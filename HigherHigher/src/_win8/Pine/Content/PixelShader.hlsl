// Per-pixel color data passed through the pixel shader.
Texture2D Texture: register(t0);
SamplerState Sampler: register(s0);
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 texcoord: TEXCOORD0;
	float map_tex : PARAM0;
};

// A pass-through function for the (interpolated) color data.
float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 color = float4(input.color[0] / 255.0f, input.color[1] / 255.0f, input.color[2] / 255.0f, input.color[3] / 255.0f);
	if (input.map_tex > 0.0f){
		float4 tcolor = Texture.Sample(Sampler, input.texcoord);
			color *= tcolor;
	}
	return color;
}
