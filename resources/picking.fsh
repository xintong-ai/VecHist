uniform int drawIndex;

void main()
{
	drawIndex = 1;
    gl_FragColor = vec4(float(drawIndex), float(0), float(0), float(1));
} 