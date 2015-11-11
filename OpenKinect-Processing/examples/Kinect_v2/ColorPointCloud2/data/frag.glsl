#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec4 vertColor;

void main() {
  //color image int BGRA Color format
  gl_FragColor = vertColor;
}
