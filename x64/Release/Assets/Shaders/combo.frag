#version 400
out vec4 FragColor;
in vec3 v_norm;
in vec4 v_pos; 

uniform vec3 lightDir;
uniform vec3 fogColor;

uniform mat4 u_pm; // uniform_ProjectionMatrix
uniform mat4 u_vm; // uniform_ViewMatrix

uniform float maxDist; //fog max distance
uniform float minDist; //fog min distance

void main() 
{
float dist = abs( v_pos.z );
float fogFactor = (minDist - dist) / (minDist - maxDist);
fogFactor = clamp(fogFactor, 0.0, 1.0);
vec3 lightColor = vec3(0.1,0.1,0.1);
vec3 color = mix(fogColor, lightColor, fogFactor);

float intensity;
vec4 toonColor;
intensity = dot(lightDir, v_norm);

if (intensity > 0.9)
		toonColor = vec4(1.0,0.9,0.0,0.9);

else if (intensity > 0.8)
		toonColor = vec4(0.9,0.8,0.0,0.8);

else if (intensity > 0.7)
		toonColor = vec4(0.8,0.7,0.0,0.8);

else if (intensity > 0.6)
		toonColor = vec4(0.7,0.6,0.0,0.7);

else if (intensity > 0.5)
		toonColor = vec4(0.6,0.5,0.0,0.6);

else if (intensity > 0.4)
		toonColor = vec4(0.5,0.4,0.0,0.5);

else if (intensity > 0.3)
		toonColor = vec4(0.4,0.3,0.0,0.4);

else if (intensity > 0.2)
		toonColor = vec4(0.3,0.2,0.0,0.3);

else if (intensity > 0.2)
		toonColor = vec4(0.2,0.1,0.0,0.2);

else
		toonColor = vec4(0.1,0.0,0.0,0.1);


FragColor = (vec4(color, 1.0) * toonColor);
}
