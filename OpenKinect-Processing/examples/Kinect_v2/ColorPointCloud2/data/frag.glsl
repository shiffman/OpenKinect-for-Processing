#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec4 vertColor;

//input color
uniform vec4 fragColor;

void main() {

  //outputColor
  //invert the color position to match the openGL color
  gl_FragColor = vec4(vertColor.b, vertColor.g, vertColor.r, 1.0);
}