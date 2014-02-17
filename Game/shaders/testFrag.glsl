# version 330 core

uniform sampler2D texture1;
uniform float time;

in Vertex {
	smooth vec4 colour;
	smooth vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void) {
	gl_FragColor = IN.colour * texture(texture1, IN.texCoord);
}