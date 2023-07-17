
#version 430 core


in vec2 poiz;
uniform float time;
uniform sampler2D renderedTexture;
uniform int type;


uniform float vx_offset = 0.5;
uniform float rt_w = 978; // GeeXLab built-in
uniform float rt_h = 486; // GeeXLab built-in
uniform float pixel_w = 15.0; // 15.0
uniform float pixel_h = 15.0; // 10.0
const float PI = 3.1415926535;
void main() 
{ 
	if(type==0)
	{
		gl_FragColor =texture2D(renderedTexture, poiz);
	}
	else if(type==1)
	{
		float aperture = 178.0;
		float apertureHalf = 0.5 * aperture * (PI / 180.0);
		float maxFactor = sin(apertureHalf);
	  
		vec2 uv;
		vec2 xy = 2.0 * poiz.xy - 1.0;
		float d = length(xy);
		if (d < (2.0-maxFactor))
		{
			d = length(xy * maxFactor);
			float z = sqrt(1.0 - d * d);
			float r = atan(d, z) / PI;
			float phi = atan(xy.y, xy.x);
		
			uv.x = r * cos(phi) + 0.5;
			uv.y = r * sin(phi) + 0.5;
		}
		else
		{
			uv = poiz.xy;
		}
		vec4 c = texture2D(renderedTexture, uv);
		gl_FragColor = c;
	}
	else if(type==2)
	{
		float mapp[25]={1.0,4.0,6.0,4.0,1.0,
							4.0,16.0,24.0,16.0,4.0,
							6.0,24.0,36.0,24.0,6.0,
							4.0,16.0,24.0,16.0,4.0,
							1.0,4.0,6.0,4.0,1.0};
		vec2 uv=poiz;
		vec4 tempcolor=vec4(0.0,0.0,0.0,1.0);
		float pixelh=1.0/486.0;
		float pixelw=1.0/978.0;
		for(int i=-2;i<3;i++)
		{
			for(int j=-2;j<3;j++)
			{
				vec4 temp=texture2D(renderedTexture,vec2(uv.x+i*pixelw,uv.y));
				temp =vec4(mapp[(i+2)*5+j+2]*temp.r,mapp[(i+2)*5+j+2]*temp.g,mapp[(i+2)*5+j+2]*temp.b,mapp[(i+2)*5+j+2]*1.0);
				tempcolor=tempcolor+temp;
			}
		}
		gl_FragColor=(tempcolor/256-texture2D(renderedTexture,uv))*2;
	}
	else if(type==3)
	{
		vec4 fcolor = texture2D(renderedTexture, poiz);
		fcolor.r=floor((fcolor.r*256.0)/32.0);
		fcolor.g=floor((fcolor.g*256.0)/32.0);
		fcolor.b=floor((fcolor.b*256.0)/64.0);
		
		
		fcolor.r = fcolor.r*32.0;
		fcolor.g =fcolor.g*32.0;
		fcolor.b = fcolor.b*64.0;
		fcolor.a = 1.0;
		fcolor.r =fcolor.r/256.0;
		fcolor.g =fcolor.g/256.0;
		fcolor.b =fcolor.b/256.0;
		gl_FragColor = fcolor;
	}
	else if(type==4)
	{
		vec2 uv = poiz;
		vec3 tc = vec3(1.0, 0.0, 0.0);
		if (uv.x < (vx_offset - 0.005))
		{
			float dx = pixel_w*(1./rt_w);
			float dy = pixel_h*(1./rt_h);
			vec2 coord = vec2(dx * floor(uv.x/dx),dy * floor(uv.y/dy));
			tc = texture2D(renderedTexture, coord).rgb;
		}
		else if (uv.x >= (vx_offset + 0.005))
		{
			tc = texture2D(renderedTexture, uv).rgb;
		}
		gl_FragColor = vec4(tc,1.0);
	}
	else if(type==5)
	{
		vec2 uv = poiz;
		vec4 tmpcolor=vec4(0.0,0.0,0.0,1.0);
		vec2 move = uv - vec2(0.5,0.5);
		//move=normalize(move);
		move = vec2(move.x/200.0,move.y/200.0);
		for(float i=0.;i<200.;i++) 
		{
			tmpcolor = tmpcolor + texture2D(renderedTexture,vec2((uv.x-move.x*i),(uv.y-move.y*i)))/400.0;
		}
		tmpcolor=tmpcolor +texture2D(renderedTexture,poiz)/2;
		
		gl_FragColor=tmpcolor;
	}
}