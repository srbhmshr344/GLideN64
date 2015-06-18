#define SHADER_VERSION "#version 100 \n"

static const char* vertex_shader =
SHADER_VERSION
"#if (__VERSION__ > 120)						\n"
"# define IN in									\n"
"# define OUT out								\n"
"#else											\n"
"# define IN attribute							\n"
"# define OUT varying							\n"
"#endif // __VERSION							\n"
"IN highp vec4 aPosition;						\n"
"IN lowp vec4 aColor;							\n"
"IN highp vec2 aTexCoord0;						\n"
"IN highp vec2 aTexCoord1;						\n"
"IN lowp float aNumLights;						\n"
"													\n"
"uniform int uRenderState;							\n"
"uniform int uTexturePersp;							\n"
"													\n"
"uniform int uFogMode;								\n"
"uniform lowp int uFogUsage;						\n"
"uniform lowp float uFogAlpha;						\n"
"uniform mediump vec2 uFogScale;					\n"
"													\n"
"uniform mediump vec2 uTexScale;						\n"
"uniform mediump vec2 uTexMask[2];						\n"
"uniform mediump vec2 uTexOffset[2];					\n"
"uniform mediump vec2 uCacheScale[2];					\n"
"uniform mediump vec2 uCacheOffset[2];					\n"
"uniform mediump vec2 uCacheShiftScale[2];				\n"
"uniform lowp ivec2 uCacheFrameBuffer;					\n"
"OUT lowp vec4 vShadeColor;							\n"
"OUT mediump vec2 vTexCoord0;						\n"
"OUT mediump vec2 vTexCoord1;						\n"
"OUT mediump vec2 vLodTexCoord;						\n"
"OUT lowp float vNumLights;							\n"
"OUT mediump float vFogFragCoord;					\n"

"mediump vec2 calcTexCoord(in vec2 texCoord, in int idx)		\n"
"{																\n"
"    vec2 texCoordOut = texCoord*uCacheShiftScale[idx];			\n"
"    if (uCacheFrameBuffer[idx] != 0) {							\n"
"      if (uTexMask[idx] != vec2(0.0, 0.0))						\n"
"        texCoordOut -= mod(uTexOffset[idx], uTexMask[idx]);	\n"
"      else														\n"
"        texCoordOut -= uTexOffset[idx];						\n"
"      texCoordOut.t = -texCoordOut.t;							\n"
"    } else														\n"
"        texCoordOut -= uTexOffset[idx];						\n"
"    return (uCacheOffset[idx] + texCoordOut)* uCacheScale[idx];\n"
"}																\n"
"																\n"
"void main()													\n"
"{																\n"
"  gl_Position = aPosition;										\n"
"  vFogFragCoord = 0.0;											\n"
"  vShadeColor = aColor;										\n"
"  if (uRenderState == 1) {										\n"
"    vec2 texCoord = aTexCoord0;								\n"
"    texCoord *= uTexScale;										\n"
"    if (uTexturePersp == 0) texCoord *= 0.5;					\n"
"    vTexCoord0 = calcTexCoord(texCoord, 0);					\n"
"    vTexCoord1 = calcTexCoord(texCoord, 1);					\n"
"    vLodTexCoord = texCoord * uCacheShiftScale[0];				\n"
"    vNumLights = aNumLights;									\n"
"    if (uFogMode == 0) {										\n"
"        if (aPosition.z < -aPosition.w)						\n"
"          vFogFragCoord = -uFogScale.s + uFogScale.t;			\n"
"        else													\n"
"          vFogFragCoord = aPosition.z/aPosition.w*uFogScale.s	\n"
"	                   + uFogScale.t;							\n"
"    } else if (uFogMode == 1) {								\n"
"          vFogFragCoord = uFogAlpha;							\n"
"    } else if (uFogMode == 2) {								\n"
"          vFogFragCoord = 1.0 - uFogAlpha;						\n"
"    }															\n"
"    vFogFragCoord = clamp(vFogFragCoord, 0.0, 1.0);			\n"
"    lowp int fogUsage = uFogUsage;								\n"
"    if (fogUsage >= 256) fogUsage -= 256;						\n"
"    if (fogUsage == 1 && uFogMode == 0)						\n"
"       vShadeColor.a = vFogFragCoord;							\n"
"  } else {														\n"
"    vTexCoord0 = aTexCoord0;									\n"
"    vTexCoord1 = aTexCoord1;									\n"
"    vNumLights = 0.0;											\n"
"    if (uFogMode == 1) vFogFragCoord = uFogAlpha;				\n"
"    else if (uFogMode == 2) vFogFragCoord = 1.0 - uFogAlpha;	\n"
"  }															\n"
"}																\n"
;

static const char* vertex_shader_notex =
SHADER_VERSION
"#if (__VERSION__ > 120)			\n"
"# define IN in						\n"
"# define OUT out					\n"
"#else								\n"
"# define IN attribute				\n"
"# define OUT varying				\n"
"#endif // __VERSION				\n"
"IN highp vec4 aPosition;			\n"
"IN lowp vec4 aColor;				\n"
"IN lowp float aNumLights;			\n"
"									\n"
"uniform int uRenderState;			\n"
"									\n"
"uniform int uFogMode;				\n"
"uniform lowp int uFogUsage;		\n"
"uniform lowp float uFogAlpha;		\n"
"uniform mediump vec2 uFogScale;	\n"
"									\n"
"OUT lowp vec4 vShadeColor;			\n"
"OUT lowp float vNumLights;			\n"
"OUT mediump float vFogFragCoord;	\n"
"																\n"
"void main()													\n"
"{																\n"
"  gl_Position = aPosition;										\n"
"  vFogFragCoord = 0.0;											\n"
"  vShadeColor = aColor;										\n"
"  if (uRenderState == 1) {										\n"
"    vNumLights = aNumLights;									\n"
"    if (uFogMode == 0) {										\n"
"        if (aPosition.z < -aPosition.w)						\n"
"          vFogFragCoord = -uFogScale.s + uFogScale.t;			\n"
"        else													\n"
"          vFogFragCoord = aPosition.z/aPosition.w*uFogScale.s	\n"
"	                   + uFogScale.t;							\n"
"    } else if (uFogMode == 1) {								\n"
"          vFogFragCoord = uFogAlpha;							\n"
"    } else if (uFogMode == 2) {								\n"
"          vFogFragCoord = 1.0 - uFogAlpha;						\n"
"    }															\n"
"    vFogFragCoord = clamp(vFogFragCoord, 0.0, 1.0);			\n"
"    lowp int fogUsage = uFogUsage;								\n"
"    if (fogUsage >= 256) fogUsage -= 256;						\n"
"    if (fogUsage == 1 && uFogMode == 0)						\n"
"       vShadeColor.a = vFogFragCoord;							\n"
"  } else {														\n"
"    vNumLights = 0.0;											\n"
"    if (uFogMode == 1) vFogFragCoord = uFogAlpha;				\n"
"    else if (uFogMode == 2) vFogFragCoord = 1.0 - uFogAlpha;	\n"
"  }															\n"
"}																\n"
;

static const char* fragment_shader_header_common_variables =
SHADER_VERSION
"#if (__VERSION__ > 120)		\n"
"# define IN in					\n"
"# define OUT out				\n"
"#else							\n"
"# define IN varying			\n"
"# define OUT					\n"
"#endif // __VERSION __			\n"
"uniform sampler2D uTex0;		\n"
"uniform sampler2D uTex1;		\n"
"uniform lowp vec4 uFogColor;	\n"
"uniform lowp vec4 uCenterColor;\n"
"uniform lowp vec4 uScaleColor;	\n"
"uniform lowp vec4 uBlendColor;	\n"
"uniform lowp vec4 uEnvColor;	\n"
"uniform lowp vec4 uPrimColor;	\n"
"uniform lowp float uPrimLod;	\n"
"uniform lowp float uK4;		\n"
"uniform lowp float uK5;		\n"
"uniform mediump vec2 uScreenScale;	\n"
"uniform lowp int uAlphaCompareMode;	\n"
"uniform lowp int uGammaCorrectionEnabled;	\n"
"uniform lowp int uFogUsage;	\n"
"uniform lowp ivec2 uFb8Bit;		\n"
"uniform lowp ivec2 uFbFixedAlpha;\n"
"uniform lowp int uSpecialBlendMode;\n"
"uniform lowp int uEnableAlphaTest;	\n"
"uniform lowp float uAlphaTestValue;\n"
"uniform mediump vec2 uDepthScale;	\n"
"IN lowp vec4 vShadeColor;	\n"
"IN mediump vec2 vTexCoord0;\n"
"IN mediump vec2 vTexCoord1;\n"
"IN mediump vec2 vLodTexCoord;\n"
"IN lowp float vNumLights;	\n"
"IN mediump float vFogFragCoord;\n"
"lowp vec3 input_color;			\n"
"OUT lowp vec4 fragColor;		\n"
"lowp int nCurrentTile;			\n"
;

static const char* fragment_shader_header_common_variables_notex =
SHADER_VERSION
"#if (__VERSION__ > 120)		\n"
"# define IN in					\n"
"# define OUT out				\n"
"#else							\n"
"# define IN varying			\n"
"# define OUT					\n"
"#endif // __VERSION __			\n"
"uniform lowp vec4 uFogColor;	\n"
"uniform lowp vec4 uCenterColor;\n"
"uniform lowp vec4 uScaleColor;	\n"
"uniform lowp vec4 uBlendColor;	\n"
"uniform lowp vec4 uEnvColor;	\n"
"uniform lowp vec4 uPrimColor;	\n"
"uniform lowp float uPrimLod;	\n"
"uniform lowp float uK4;		\n"
"uniform lowp float uK5;		\n"
"uniform mediump vec2 uScreenScale;	\n"
"uniform lowp int uAlphaCompareMode;	\n"
"uniform lowp int uGammaCorrectionEnabled;	\n"
"uniform lowp int uFogUsage;	\n"
"uniform lowp int uSpecialBlendMode;\n"
"uniform lowp int uEnableAlphaTest;	\n"
"uniform lowp float uAlphaTestValue;\n"
"uniform mediump vec2 uDepthScale;	\n"
"IN lowp vec4 vShadeColor;	\n"
"IN lowp float vNumLights;	\n"
"IN mediump float vFogFragCoord;\n"
"lowp vec3 input_color;			\n"
"OUT lowp vec4 fragColor;		\n"
;

static const char* fragment_shader_header_common_functions =
"															\n"
"lowp float snoise();						\n"
"void calc_light(in lowp float fLights, in lowp vec3 input_color, out lowp vec3 output_color);\n"
"mediump float mipmap(out lowp vec4 readtex0, out lowp vec4 readtex1);		\n"
"lowp vec4 readTex(in sampler2D tex, in mediump vec2 texCoord, in bool fb8bit, in bool fbFixedAlpha);	\n"
#ifdef USE_TOONIFY
"void toonify(in mediump float intensity);	\n"
#endif
;

static const char* fragment_shader_header_common_functions_notex =
"															\n"
"lowp float snoise();						\n"
"void calc_light(in lowp float fLights, in lowp vec3 input_color, out lowp vec3 output_color);\n"
;

static const char* fragment_shader_calc_light =
"uniform mediump vec3 uLightDirection[8];	\n"
"uniform lowp vec3 uLightColor[8];			\n"
"void calc_light(in lowp float fLights, in lowp vec3 input_color, out lowp vec3 output_color) {\n"
"  output_color = input_color;									\n"
"  lowp int nLights = int(floor(fLights + 0.5));				\n"
"  if (nLights == 0)											\n"
"     return;													\n"
"  output_color = uLightColor[nLights];							\n"
"  mediump float intensity;										\n"
"  mediump vec3 n = normalize(input_color);						\n"
"  for (int i = 0; i < nLights; i++)	{						\n"
"    intensity = max(dot(n, uLightDirection[i]), 0.0);			\n"
"    output_color += intensity*uLightColor[i];					\n"
"  };															\n"
"  clamp(output_color, 0.0, 1.0);								\n"
"}																\n"
;

static const char* fragment_shader_header_main =
"									\n"
"void main()						\n"
"{									\n"
"  lowp vec4 vec_color, combined_color;	\n"
"  lowp float alpha1, alpha2;			\n"
"  lowp vec3 color1, color2;				\n"
;

#ifdef USE_TOONIFY
static const char* fragment_shader_toonify =
"																	\n"
"void toonify(in mediump float intensity) {							\n"
"   if (intensity > 0.5)											\n"
"	   return;														\n"
"	else if (intensity > 0.125)										\n"
"		fragColor = vec4(vec3(fragColor)*0.5, fragColor.a);\n"
"	else															\n"
"		fragColor = vec4(vec3(fragColor)*0.2, fragColor.a);\n"
"}																	\n"
;
#endif

static const char* fragment_shader_end =
"}                               \n"
;

static const char* fragment_shader_fake_mipmap =
"uniform lowp int uMaxTile;			\n"
"uniform mediump float uMinLod;		\n"
"														\n"
"mediump float mipmap(out lowp vec4 readtex0, out lowp vec4 readtex1) {	\n"
"  readtex0 = texture2D(uTex0, vTexCoord0);				\n"
"  readtex1 = texture2D(uTex1, vTexCoord1);				\n"
"  if (uMaxTile == 0) return 1.0;						\n"
"  return uMinLod;										\n"
"}														\n"
;

static const char* fragment_shader_readtex =
"lowp vec4 readTex(in sampler2D tex, in mediump vec2 texCoord, in bool fb8bit, in bool fbFixedAlpha)	\n"
"{																			\n"
"  lowp vec4 texColor = texture2D(tex, texCoord); 							\n"
"  if (fb8bit) texColor = vec4(texColor.r);									\n"
"  if (fbFixedAlpha) texColor.a = 0.825;									\n"
"  return texColor;															\n"
"}																			\n"
;

static const char* fragment_shader_readtex_3point =
"uniform mediump vec2 uTextureSize[2];								\n"
"uniform lowp int uTextureFilterMode;								\n"
// 3 point texture filtering.
// Original author: ArthurCarvalho
// GLSL implementation: twinaphex, mupen64plus-libretro project.
"#define TEX_OFFSET(off) texture2D(tex, texCoord - (off)/texSize)	\n"
"lowp vec4 filter3point(in sampler2D tex, in mediump vec2 texCoord)			\n"
"{																			\n"
"  mediump vec2 texSize = uTextureSize[nCurrentTile];						\n"
"  mediump vec2 offset = fract(texCoord*texSize - vec2(0.5));	\n"
"  offset -= step(1.0, offset.x + offset.y);								\n"
"  lowp vec4 c0 = TEX_OFFSET(offset);										\n"
"  lowp vec4 c1 = TEX_OFFSET(vec2(offset.x - sign(offset.x), offset.y));	\n"
"  lowp vec4 c2 = TEX_OFFSET(vec2(offset.x, offset.y - sign(offset.y)));	\n"
"  return c0 + abs(offset.x)*(c1-c0) + abs(offset.y)*(c2-c0);				\n"
"}																			\n"
"lowp vec4 readTex(in sampler2D tex, in mediump vec2 texCoord, in bool fb8bit, in bool fbFixedAlpha)	\n"
"{																			\n"
"  lowp vec4 texStandard = texture2D(tex, texCoord); 							\n"
"  lowp vec4 tex3Point = filter3point(tex, texCoord); 						\n"
"  lowp vec4 texColor = uTextureFilterMode == 0 ? texStandard : tex3Point;	\n"
"  if (fb8bit) texColor = vec4(texColor.r);									\n"
"  if (fbFixedAlpha) texColor.a = 0.825;									\n"
"  return texColor;															\n"
"}																			\n"
;

static const char* fragment_shader_noise =
"uniform sampler2D uTexNoise;		\n"
"lowp float snoise()									\n"
"{														\n"
"  mediump vec2 texSize = vec2(640.0, 580.0);						\n"
"  mediump vec2 coord = gl_FragCoord.xy/uScreenScale/texSize;		\n"
"  return texture2D(uTexNoise, coord).r;							\n"
"}														\n"
;

static const char* fragment_shader_dummy_noise =
"						\n"
"lowp float snoise()	\n"
"{						\n"
"  return 1.0;			\n"
"}						\n"
;

static const char* default_vertex_shader =
SHADER_VERSION
"#if (__VERSION__ > 120)						\n"
"# define IN in									\n"
"#else											\n"
"# define IN attribute							\n"
"#endif // __VERSION							\n"
"IN highp vec4 	aPosition;										\n"
"void main()                                                    \n"
"{                                                              \n"
"  gl_Position = aPosition;										\n"
"}                                                              \n"
;

static const char* zelda_monochrome_fragment_shader =
SHADER_VERSION
"uniform sampler2D uColorImage;									\n"
"uniform mediump vec2 uScreenSize;								\n"
"void main()													\n"
"{																\n"
"  mediump vec2 coord = gl_FragCoord.xy/uScreenSize;			\n"
"  lowp vec4 tex = texture2D(uColorImage, coord);				\n"
"  lowp float c = dot(vec4(0.2126, 0.7152, 0.0722, 0.0), tex);	\n"
"  gl_FragColor = vec4(c, c, c, 1.0);							\n"
"}																\n"
;
