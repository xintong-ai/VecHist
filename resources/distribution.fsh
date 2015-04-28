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

varying vec3 position, normal, texcoord;
//varying vec4 specular, ambient, diffuse, lightDirection;
varying vec4 lightDirection;

//uniform sampler2D tex;
uniform samplerCube env;
//uniform mat4 view;

vec4 GetColor(float v,float vmin,float vmax)
{
   vec4 c = vec4(1.0,1.0,1.0,1.0); // white
   float dv;
   if (v < vmin)
      v = vmin;
   if (v > vmax)
      v = vmax;
   dv = vmax - vmin;

   if (v < (vmin + 0.25 * dv)) {
      c.r = 0.0f;
      c.g = 4.0f * (v - vmin) / dv;
   } else if (v < (vmin + 0.5 * dv)) {
      c.r = 0.0f;
      c.b = 1.0f + 4.0f * (vmin + 0.25 * dv - v) / dv;
   } else if (v < (vmin + 0.75 * dv)) {
      c.r = 4.0f * (v - vmin - 0.5 * dv) / dv;
      c.b = 0.0f;
   } else {
      c.g = 1.0f + 4.0f * (vmin + 0.75 * dv - v) / dv;
      c.b = 0.0f;
   }

   return(c);
}

void main()
{
    //gl_FragColor = textureCube(env, normal);
	float v = textureCube(env, texcoord).x;
	vec4 unlitColor = GetColor(v, 0, 0.02); //vec4((normal.x + 1) * 0.5, (normal.y + 1), (normal.z + 1) * 0.5, 1.0f);
	
	
		
    vec3 N = normalize(normal);
    // assume directional light

    gl_MaterialParameters M = gl_FrontMaterial;
	
	//vec3 tmp = vec3(0.0, 1.0, 0.0);
	//lightDirection = vec4(1.0, 0.0, 0.0,0.0);
    float NdotL = dot(N, lightDirection.xyz);
	//
    float RdotL = dot(reflect(normalize(position), N), lightDirection.xyz);

    //vec4 unlitColor = mix(marbleColors[0], marbleColors[1], exp(-4.0 * abs(turbulence)));
	vec4 ambient = vec4(0.3, 0.3, 0.3 ,1.0);
	vec4 diffuse = vec4(0.3, 0.3, 0.3 ,1.0);
	vec4 specular = vec4(0.3, 0.3, 0.3 ,1.0);
	gl_FragColor = unlitColor;
	// TODO: the light direction needs to be fixed
    //gl_FragColor = (ambient + diffuse * max(NdotL, 0.0)) * unlitColor +
    //                specular * pow(max(RdotL, 0.0), 20);//M.specular * //M.shininess
}
