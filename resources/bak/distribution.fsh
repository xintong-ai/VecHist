/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

varying vec4 myposition;
varying vec3 mynormal;
varying vec3 texcoord;
//varying vec4 specular, ambient, diffuse, lightDirection;
//varying vec4 lightDirection;

//uniform sampler2D tex;
uniform samplerCube env;
//uniform mat4 view;

uniform vec4 lightposn[2];
uniform vec4 ambient;
uniform vec4 diffuse; 
uniform vec4 specular; 
uniform vec4 emission; 
uniform float shininess;
uniform vec4 cm[33];

vec4 GetColor(float v,float vmin,float vmax)
{
   //vec4 c = vec4(1.0,1.0,1.0,1.0); // white
   vec4 c = vec4(0.0, 0.0, 0.0, 1.0);
   float dv;
   //if (v < vmin) {
   //   v = vmin;
   //}
   //if (v > vmax) {
   //   v = vmax;
   //}
   dv = vmax - vmin;

   //if (v < (vmin + 0.25 * dv)) {
   //   c.r = 0.0f;
   //   c.g = 4.0f * (v - vmin) / dv;
   //} else if (v < (vmin + 0.5 * dv)) {
   //   c.r = 0.0f;
   //   c.b = 1.0f + 4.0f * (vmin + 0.25 * dv - v) / dv;
   //} else if (v < (vmin + 0.75 * dv)) {
   //   c.r = 4.0f * (v - vmin - 0.5 * dv) / dv;
   //   c.b = 0.0f;
   //} else {
   //   c.g = 1.0f + 4.0f * (vmin + 0.75 * dv - v) / dv;
   //   c.b = 0.0f;
   //}

   //if (v < (vmin + 0.5 * dv)) {
	//c.r = 0.231373 + (v - vmin) * 2 / dv * (0.865003 - 0.231373);
	//c.g = 0.298039 + (v - vmin) * 2 / dv * (0.865003 - 0.298039);
	//c.b = 0.752941 + (v - vmin) * 2 / dv * (0.865003 - 0.752941);
   //} else {
	//c.r = 0.865003 - (v - 0.5 * dv) * 2 / dv * (0.865003 - 0.705882);
	//c.g = 0.865003 - (v - 0.5 * dv) * 2 / dv * (0.865003 - 0.0156863);
	//c.b = 0.865003 - (v - 0.5 * dv) * 2 / dv * (0.865003 - 0.14902);
   //}

/*
   //intepolate in the Msh color space
   float M, s, h;
   
   if (v < (vmin + 0.5 * dv)) {
	M = 80 + (v - vmin) * 2 / dv * (88 - 80);
	s = 1.08 + (v - vmin) * 2 / dv * (0 - 1.08);
	h = -1.1 + (v - vmin) * 2 / dv * (-1.661 + 1.1);
   } else {
	M = 88 + (v - vmin - 0.5 * dv) * 2 / dv * (80 - 88);
	s = 0 + (v - vmin - 0.5 * dv) * 2 / dv * (1.08 - 0);
	h = 1.061 + (v - vmin - 0.5 * dv) * 2 / dv * (0.5 - 1.061);
	//h = -1.661 + (v - vmin - 0.5 * dv) * 2 / dv * (0.5 + 1.661);
   }

   //transfer to CIELAB color space
   float l, a, b;
   
   l = M * cos(s);
   a = M * sin(s) * cos(h);
   b = M * sin(s) * sin(h);

   //transfer to CIEXYZ color space
   float cx, cy, cz, fx, fy, fz;
   fy = (l + 16) / 116;
   fx = fy + a / 500;
   fz = fy - b / 200;

   if (fy > 0.206897) {
	cy = fy * fy * fy;
   } else {
	cy = (fy - 0.137931) * 0.128418;
   }

   if (fx > 0.206897) {
	cx = fx * fx * fx;
   } else {
	cx = (fx - 0.137931) * 0.128418;
   }

   if (fz > 0.206897) {
	cz = fz * fz * fz;
   } else {
	cz = (fz - 0.137931) * 0.128418;
   }

   //transfer to RGB color space
   c.r = 3.0799327 * cx - 1.537150 * cy - 0.542782 * cz;
   c.g = -0.921235 * cx + 1.875992 * cy + 0.0452442 * cz;
   c.b = 0.0528909 * cx - 0.204043 * cy + 1.1511515 * cz;

   if (c.r > 1.0)
	c.r = 1.0;
   else if (c.r < 0.0)
	c.r = 0.0;
   
   if (c.g > 1.0)
	c.g = 1.0;
   else if (c.g < 0.0)
	c.g = 0.0;

   if (c.b > 1.0)
	c.b = 1.0;
   else if (c.b < 0.0)
	c.b = 0.0;
*/

/*
   vec3 cm[33];
   cm[0] =  vec3(0.2298057, 0.298717966, 0.753683153);
   cm[1] =  vec3(0.26623388, 0.353094838, 0.801466763);
   cm[2] =  vec3(0.30386891, 0.406535296, 0.84495867);
   cm[3] =  vec3(0.342804478, 0.458757618, 0.883725899);
   cm[4] =  vec3(0.38301334, 0.50941904, 0.917387822);
   cm[5] =  vec3(0.424369608, 0.558148092, 0.945619588);
   cm[6] =  vec3(0.46666708, 0.604562568, 0.968154911);
   cm[7] =  vec3(0.509635204, 0.648280772, 0.98478814);
   cm[8] =  vec3(0.552953156, 0.688929332, 0.995375608);
   cm[9] =  vec3(0.596262162, 0.726149107, 0.999836203);
   cm[10] = vec3(0.639176211, 0.759599947, 0.998151185);
   cm[11] = vec3(0.681291281, 0.788964712, 0.990363227);
   cm[12] = vec3(0.722193294, 0.813952739, 0.976574709);
   cm[13] = vec3(0.761464949, 0.834302879, 0.956945269);
   cm[14] = vec3(0.798691636, 0.849786142, 0.931688648);
   cm[15] = vec3(0.833466556, 0.860207984, 0.901068838);
   cm[16] = vec3(0.865395197, 0.86541021, 0.865395561);
   cm[17] = vec3(0.897787179, 0.848937047, 0.820880546);
   cm[18] = vec3(0.924127593, 0.827384882, 0.774508472);
   cm[19] = vec3(0.944468518, 0.800927443, 0.726736146);
   cm[20] = vec3(0.958852946, 0.769767752, 0.678007945);
   cm[21] = vec3(0.96732803, 0.734132809, 0.628751763);
   cm[22] = vec3(0.969954137, 0.694266682, 0.579375448);
   cm[23] = vec3(0.966811177, 0.650421156, 0.530263762);
   cm[24] = vec3(0.958003065, 0.602842431, 0.481775914);
   cm[25] = vec3(0.943660866, 0.551750968, 0.434243684);
   cm[26] = vec3(0.923944917, 0.49730856, 0.387970225);
   cm[27] = vec3(0.89904617, 0.439559467, 0.343229596);
   cm[28] = vec3(0.869186849, 0.378313092, 0.300267182);
   cm[29] = vec3(0.834620542, 0.312874446, 0.259301199);
   cm[30] = vec3(0.795631745, 0.24128379, 0.220525627);
   cm[31] = vec3(0.752534934, 0.157246067, 0.184115123);
   cm[32] = vec3(0.705673158, 0.01555616, 0.150232812);
*/

   float pdv = 0.03125 * dv;

   int i = 0;
   for (i = 0; i < 32; i++) {
	if (v >= (vmin + i*pdv) && v < (vmin + (i+1)*pdv)) {
	    c.r = cm[i].r + (v - vmin - i*pdv) / pdv * (cm[i+1].r - cm[i].r);
	    c.g = cm[i].g + (v - vmin - i*pdv) / pdv * (cm[i+1].g - cm[i].g);
   	    c.b = cm[i].b + (v - vmin - i*pdv) / pdv * (cm[i+1].b - cm[i].b);

	    break;
	}
   }

   if (v == vmax) {
	c.r = cm[32].r;
	c.g = cm[32].g;
	c.b = cm[32].b;
   }


   return(c);
}

vec4 ComputeLight(const in vec3 direction, const in vec4
	lightcolor, const in vec3 normal, const in vec3 halfvec, const
	in vec4 mydiffuse, const in vec4 myspecular, const in float
	myshininess) {

	float nDotL = dot(normal, direction);
	vec4 lambert = mydiffuse * lightcolor * max(nDotL, 0.0);

	float nDotH = dot(normal, halfvec);
	vec4 phong = myspecular * lightcolor * pow(max(nDotH, 0.0), 
					myshininess);

	vec4 retval = lambert + phong;
	return retval;
}

void main()
{
    //gl_FragColor = textureCube(env, normal);
	float v = textureCube(env, texcoord).x;
	vec4 unlitColor = GetColor(v, 0, 1); //vec4((normal.x + 1) * 0.5, (normal.y + 1), (normal.z + 1) * 0.5, 1.0f);
	
    vec4 final_color = vec4(0.0, 0.0, 0.0, 1.0);
    for(int i = 0; i < 2; i++){
	const vec3 eyepos = vec3(0, 0, 0);
	vec4 _mypos = gl_ModelViewMatrix * myposition;
	vec3 mypos = _mypos.xyz / _mypos.w;
	vec3 eyedirn = normalize(eyepos - mypos);
	vec4 _normal = gl_ModelViewMatrixInverseTranspose*vec4(mynormal,0.0);
	vec3 normal = vec3(_normal[0], _normal[1], _normal[2]);
	normal = normalize(normal);
	 
	vec3 direction;
	if(lightposn[i][3] == 0){
	    direction = vec3(lightposn[i][0], lightposn[i][1], lightposn[i][2]);
	    direction = normalize(direction);
	}
	else{
	    vec3 position = lightposn[i].xyz / lightposn[i].w;
	    direction = normalize(position - mypos);
	}
	vec3 _half = normalize(direction + eyedirn);
	final_color = final_color + ComputeLight(direction, unlitColor, normal, _half, diffuse, specular, shininess);
    }

    final_color = final_color + emission + ambient;
    gl_FragColor = final_color;

    // assume directional light

    //gl_MaterialParameters M = gl_FrontMaterial;
	
	//vec3 tmp = vec3(0.0, 1.0, 0.0);
	//lightDirection = vec4(1.0, 0.0, 0.0,0.0);
    //float NdotL = dot(N, lightDirection.xyz);
	//
    //float RdotL = dot(reflect(normalize(position), N), lightDirection.xyz);

    //vec4 unlitColor = mix(marbleColors[0], marbleColors[1], exp(-4.0 * abs(turbulence)));
	//vec4 ambient = vec4(0.1, 0.1, 0.1 ,1.0);
	//vec4 diffuse = vec4(0.6, 0.6, 0.6 ,1.0);
	//vec4 specular = vec4(0.1, 0.1, 0.1 ,1.0);
	//gl_FragColor = unlitColor;
	// TODO: the light direction needs to be fixed
    //gl_FragColor = ambient + diffuse * max(NdotL, 0.0) * unlitColor +
      //              specular * pow(max(RdotL, 0.0), 20) * unlitColor;//M.specular * //M.shininess
}
