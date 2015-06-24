#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

#define PROCESSING_TEXTURE_SHADER

uniform sampler2D texture;

void main()
{

  vec4 col0 = texture2D(texture, vertTexCoord.st);

  gl_FragColor = col0;

}