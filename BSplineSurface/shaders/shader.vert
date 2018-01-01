#version 330 core
layout(location = 0) in vec3 a_Position; //������
layout(location = 1) in vec3 a_Normal;   //�㷨��
//layout(location = 2) in vec3 a_Color;    //����ɫ
out vec4 v_Color;   //�����ɫ

uniform vec3 u_DiffuseLight;   //�������
uniform vec3 u_LightDirection; //���߷���
uniform vec3 u_AmbientLight;   //������

uniform mat4 u_Model;
uniform mat4 u_Normal;  //model�任�󣬷���任����
uniform mat4 u_View;
uniform mat4 u_Projection;
void main()
{
    vec3 a_Color = vec3(1.0, 0.0, 0.0);
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0); 
	vec3 normal = normalize(vec3(u_Normal * vec4(a_Normal,0.0))); //����任����
	float nDotL = max(dot(u_LightDirection, normal),0);   //���㷨������߷����ڻ�
	vec3 diffuseColor = u_DiffuseLight * a_Color.rgb * nDotL;   //������ɫ

	vec3 ambient = u_AmbientLight * a_Color.rgb; 

	v_Color = vec4(diffuseColor + ambient, 1.0);

    
};
