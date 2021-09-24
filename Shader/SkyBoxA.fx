//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// SkyBox
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string SkyBox_Pass_0_Box : ModelData = ".\\Box.x";

float4 ViewPosition : ViewPosition;
float4x4 ViewProjection : ViewProjection;

struct VS_INPUT
{
    float4 position : POSITION0;
};

struct VS_OUTPUT
{
    float4 position : POSITION0;
    float3 position3D : TEXCOORD0;
};

// Constructs a translation matrix
float4x4 translation(float3 position)
{
    return float4x4(
        float4(1,0,0,0),
        float4(0,1,0,0),
        float4(0,0,1,0),
        float4(position.xyz, 1)
    );
}

VS_OUTPUT SkyBox_Pass_0_Vertex_Shader_vs_main(VS_INPUT input)
{
    VS_OUTPUT output;

    // Create a world matrix that translates the skybox to the camera position
    float4x4 world = translation(ViewPosition.xyz);

    // Combine it with the existing view projection matrix
    float4x4 WorldViewProjection = mul(world, ViewProjection);

    // Transform your vertex into place
    output.position = mul(input.position, WorldViewProjection);

    // Save the original vertex position in model space
    // Since the cube model is centered around the origin, you can use these
    // vertex positions directly as directions for your cubemap :)
    output.position3D = input.position;

    return output;
}


texture CubeTexture_Tex
<
   string ResourceName = "D:\\RenderMonkey\\Examples\\Media\\Textures\\Snow.dds";
>;
sampler CubeSampler = sampler_state
{
   Texture = (CubeTexture_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MAGFILTER = LINEAR;
};

struct SkyBox_Pass_0_Pixel_Shader_VS_OUTPUT
{
    float4 position : POSITION0;
    float3 position3D : TEXCOORD0;
};

float4 SkyBox_Pass_0_Pixel_Shader_ps_main(SkyBox_Pass_0_Pixel_Shader_VS_OUTPUT input) : COLOR0
{
    // Use the original vertex position to sample the texture cube
    return texCUBE(CubeSampler, input.position3D);
}
//--------------------------------------------------------------//
// Technique Section for SkyBox
//--------------------------------------------------------------//
technique SkyBox
{
   pass Pass_0
   {
      CULLMODE = CW;
      ZWRITEENABLE = FALSE;

      VertexShader = compile vs_2_0 SkyBox_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 SkyBox_Pass_0_Pixel_Shader_ps_main();
   }

}

