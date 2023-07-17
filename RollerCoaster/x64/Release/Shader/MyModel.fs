#version 430 core

in vec3 vs_normal;
in vec2 vs_uv;
in vec3 BaryPos;
in vec3 tolight;
in vec3 halfff;

uniform sampler2D text;
uniform float shiness = 51.2f;
uniform int isToon;

out vec4 fcolor;

const vec4 BorderColor	= vec4(0, 0, 0, 1);
vec4 AmbientLightColor	= vec4(0.4, 0.4, 0.4, 1);
vec4 DiffuseLightColor	= vec4(0.8, 0.8, 0.8, 1);
vec4 SpecularLightColor	= vec4(1, 1, 1, 1);

float edgeFactor()
{
	vec3 d = fwidth(BaryPos);
	vec3 a3 = smoothstep(vec3(0.0), d * 0.8f, BaryPos);
	return min(min(a3.x, a3.y), a3.z);
}

void main(void)
{
vec3 color_ambient = vec3(0.24725,0.1995,0.0745);
vec3 color_diffude = vec3(0.75164,0.60648,0.22648);
vec3 color_specular = vec3(0.6288,0.5558,0.36607);
	if(isToon==3||isToon==4)
	{
	AmbientLightColor	= vec4(0.8, 0.8, 0.8, 1);
	DiffuseLightColor	= vec4(0.9, 0.9, 0.9, 1);
	}

	vec4 TextureColor = vec4(0,0,0,1);
	if(isToon==0||isToon==1||isToon==4)
	{
		TextureColor=texture2D(text,vs_uv);
	}
	vec4 OutColor = vec4(0, 0, 0, 1);

	float diff = max(0, dot(vs_normal, tolight));

	if(isToon==0)
	{
		OutColor=TextureColor;
	}
	//////////////////////////////////////////////////////////////////////////
	// 算顏色 (Illumination = Ka * Ia + Kd * Id + Ks * Is)
	//////////////////////////////////////////////////////////////////////////
	// Ambient
	if(isToon !=0)
		OutColor +=	AmbientLightColor * vec4(color_ambient, 1) + AmbientLightColor * TextureColor;

	// Diffuse
	if(isToon !=0)
		OutColor += diff * DiffuseLightColor * vec4(color_diffude, 1) +
					diff * DiffuseLightColor * TextureColor;

	// Specular
	if(isToon !=0)
	{
		vec3 vReflection = normalize(reflect(-tolight, vs_normal));						// 算反射角
		
		//float spec = max(0.0, dot(vs_normal, vReflection));										// 算反射係數
		float spec = max(0.0, dot(vs_normal, halfff));										// 算反射係數
		
		if(diff != 0)
		{
			spec = pow(spec, shiness);
			OutColor += SpecularLightColor * vec4(color_specular, 1) * spec +
						SpecularLightColor * TextureColor * spec;
		}
	}
	vec3 N = normalize(vs_normal);
	vec3 L = tolight;
	float tc=pow(max(0.0,dot(N,L)),5.0);
	if(isToon==3)
	{
		if(tc>0.66)
		{
		OutColor = vec4(color_diffude,1.0);
		}
		else if(tc>0.3)
		{
		OutColor =vec4(color_diffude*0.66,1.0);
		}
		else
		{
		OutColor =vec4(color_diffude*0.33,1.0);
		}
	}
	if(isToon==4)
	{
		if(tc>0.66)
		{
		OutColor = TextureColor;
		}
		else if(tc>0.3)
		{
		OutColor = TextureColor*0.66;
		}
		else
		{
		OutColor = TextureColor*0.33;
		}
	}
	fcolor = OutColor;
}