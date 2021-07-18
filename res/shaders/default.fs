#version 330

in vec2 fragTexCoord;

out vec4 finalColor;

void main()
{
	vec2 screen = fragTexCoord.xy - vec2(0.5,0.5);	
  vec3 col = 0.5 + 0.5*cos(seconds+screen.xyx+vec3(0,2,4));
  finalColor = vec4(col,1.0);
}
