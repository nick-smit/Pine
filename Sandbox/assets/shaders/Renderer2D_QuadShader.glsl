#type vertex
#version 450 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;

struct VertexOutput {
  vec4 Color;
  vec2 TexCoords;
};

layout (location = 0) out VertexOutput Output;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjectionMatrix;

void main() {
  gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0f);
  Output.Color = a_Color;
  Output.TexCoords = a_TexCoords;
}

#type fragment
#version 450 core

layout (location = 0) out vec4 o_Color;

struct VertexOutput {
  vec2 TexCoords;
};

layout (location = 0) in VertexOutput Input;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform float u_TilingFactor;

void main() {
  o_Color = texture(u_Texture, Input.TexCoords * u_TilingFactor) * u_Color;
}
