# version 440 core

uniform sampler2D texture0;

uniform vec3 cameraPos;

uniform vec3 lightColour[10];
uniform vec3 lightPos[10];
uniform float lightRadius[10];
uniform int lightCount;
uniform float time;

in Vertex {
	vec3 worldPos;
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
} IN;

out vec4 gl_FragColor;

void main(void) {
	vec3 finalColour = vec3(0, 0, 0);
	vec4 texCol = texture(texture0, IN.texCoord);
	
	for(int i = 0; i < lightCount; i++) {
		vec3 incident = normalize(lightPos[i] - IN.worldPos);
		vec3 viewDir = normalize(cameraPos - IN.worldPos);
		vec3 halfDir = normalize(incident + viewDir);

		float dist = length(lightPos[i] - IN.worldPos);
		float atten = 1.0 - clamp(dist / (lightRadius[i] * lightRadius[i]), 0.0, 1.0);

		float lambert = max(0.0, dot(incident, IN.normal));

		float rFactor = max(0.0, dot(halfDir, IN.normal));
		float sFactor = pow(rFactor, 50.0);

		vec3 ambient = texCol.rgb * lightColour[i] * 0.1;
		vec3 diffuse = texCol.rgb * lightColour[i] * lambert * atten;
		vec3 specular = lightColour[i] * sFactor * atten;
		
		finalColour += vec3(ambient + diffuse + specular);
	}
	
	gl_FragColor = vec4(finalColour, texCol.w);
}