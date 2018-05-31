//
//  Filter.h
//  TestShader
//
//  Created by lidong on 15/9/8.
//
//

#ifndef TestShader_Filter_h
#define TestShader_Filter_h
#include "cocos2d.h"

//LOMO
const float colormatrix_lomo[] = {
    1.7f,  0.1f, 0.1f, 0, -73.1f/255,
    0,  1.7f, 0.1f, 0, -73.1f/255,
    0,  0, 1.6f, 0, -73.1f/255,
    0,  0, 0, 1.0f, 0 };

//黑白
const float colormatrix_heibai[] = {
    0.8,  1.6, 0.2, 0.0, -163.9f/255,
    0.8,  1.6, 0.2, 0.0, -163.9f/255,
    0.8,  1.6, 0.2, 0.0, -163.9/255,
    0.0,  0.0, 0.0, 1.0, 0.0 };
//复古
const float colormatrix_huajiu[] = {
    0.2,0.5, 0.1, 0.0, 40.8f/255,
    0.2, 0.5, 0.1, 0.0, 40.8f/255,
    0.2,0.5, 0.1, 0.0, 40.8f/255,
    0.0, 0.0, 0.0, 1.0, 0.0 };

//哥特
const float colormatrix_gete[] = {
    1.9,-0.3, -0.2, 0,-87.0f/255,
    -0.2, 1.7, -0.1, 0, -87.0f/255,
    -0.1,-0.6, 2.0, 0, -87.0f/255,
    0.0, 0.0, 0.0, 1.0, 0.0 };

//锐化
const float colormatrix_ruise[] = {
    4.8,-1.0, -0.1, 0.0,-388.4f/255,
    -0.5,4.4, -0.1, 0.0,-388.4f/255,
    -0.5,-1.0, 5.2, 0.0,-388.4f/255,
    0.0, 0.0, 0.0, 1.0, 0.0 };


//淡雅
const float colormatrix_danya[] = {
    0.6,0.3, 0.1, 0,73.3f/255,
    0.2,0.7, 0.1, 0,73.3f/255,
    0.2,0.3, 0.4, 0,73.3f/255,
    0.0, 0.0, 0.0, 1.0, 0.0 };

//酒红
const float colormatrix_jiuhong[] = {
    1.2,0.0, 0.0, 0.0,0.0,
    0.0,0.9, 0.0, 0.0,0.0,
    0.0,0.0, 0.8, 0.0,0.0,
    0.0, 0.0, 0.0, 1.0, 0.0 };

//清宁
const float colormatrix_qingning[] = {
    0.9, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.1,0.0, 0.0, 0.0,
    0.0, 0.0, 0.9, 0.0, 0.0,
    0.0, 0.0, 0.0, 1.0, 0.0 };

//浪漫
const float colormatrix_langman[] = {
    0.9, 0.0, 0.0, 0.0, 63.0f/255,
    0.0, 0.9,0.0, 0.0, 63.0f/255,
    0.0, 0.0, 0.9, 0.0, 63.0f/255,
    0.0, 0.0, 0.0, 1.0, 0.0 };

//光晕
const float colormatrix_guangyun[] = {
    0.9, 0.0,0.0,0.0, 64.9f/255,
    0.0, 0.9,0.0,0.0, 64.9f/255,
    0.0, 0.0,0.9,0.0, 64.9f/255,
    0.0, 0.0,0.0,1.0, 0.0 };

//蓝调
const float colormatrix_landiao[] = {
    2.1, -1.4, 0.6, 0.0, -31.0f/255,
    -0.3, 2.0, -0.3, 0.0, -31.0f/255,
    -1.1, -0.2, 2.6, 0.0, -31.0f/255,
    0.0, 0.0, 0.0, 1.0, 0.0
};

//梦幻
const float colormatrix_menghuan[] = {
    0.8, 0.3, 0.1, 0.0, 46.5f/255,
    0.1, 0.9, 0.0, 0.0, 46.5f/255,
    0.1, 0.3, 0.7, 0.0, 46.5f/255,
    0.0, 0.0, 0.0, 1.0, 0.0
};

//夜色
const float colormatrix_yese[] = {
    1.0, 0.0, 0.0, 0.0, -66.6f/255,
    0.0, 1.1, 0.0, 0.0, -66.6f/255,
    0.0, 0.0, 1.0, 0.0, -66.6f/255,
    0.0, 0.0, 0.0, 1.0, 0.0
};


#define STRINGIFY(A)  #A

static const GLchar* str_VSH =STRINGIFY(
    attribute vec4 a_position;
    attribute vec2 a_texCoord;
    attribute vec4 a_color;
    #ifdef GL_ES
      varying lowp vec4 v_fragmentColor;
      varying mediump vec2 v_texCoord;
    #else
      varying vec4 v_fragmentColor;
      varying vec2 v_texCoord;
    #endif
    void main()
    {
         gl_Position = CC_PMatrix * a_position;
         v_fragmentColor = a_color;
         v_texCoord = a_texCoord;
    }
);

static const GLchar* strFSH_ORGIN=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
    void main()
    {
       vec4 lColor=texture2D(CC_Texture0, v_texCoord);
       gl_FragColor =lColor;
    }
);



static const GLchar* strFSH_LOMO=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;

    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
        
        lColor.r = 1.7 * lColor.r + 0.1 * lColor.g + 0.1 * lColor.b -73.1/255.0;
        lColor.g = 0.0 * lColor.r + 1.7 * lColor.g + 0.1 * lColor.b -73.1/255.0;
        lColor.b = 0.0 * lColor.r + 0.0 * lColor.g + 1.6 * lColor.b -73.1/255.0;

        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_HEIBAI=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
    const vec4 W = vec4(0.2125, 0.7154, 0.0721, 0);
    void main()
    {
       vec4 col = texture2D(CC_Texture0, v_texCoord.st);
       float lum = dot(col, W);
       if (0.5 < lum) {
           gl_FragColor = v_fragmentColor;
        } else {
          gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }
);

static const GLchar* strFSH_HEIBAI2=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                           
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                               
        lColor.r = 0.8 * lColor.r + 1.6 * lColor.g + 0.2 * lColor.b  -163.9/255.0;
        lColor.g = 0.8 * lColor.r + 1.6 * lColor.g + 0.2 * lColor.b  -163.9/255.0;
        lColor.b = 0.8 * lColor.r + 1.6 * lColor.g + 0.2 * lColor.b  -163.9/255.0;
                                               
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_FUGU=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                             
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                                 
        lColor.r = 0.2 * lColor.r + 0.5 * lColor.g + 0.1 * lColor.b  +40.8/255.0;
        lColor.g = 0.2 * lColor.r + 0.5 * lColor.g + 0.1 * lColor.b  +40.8/255.0;
        lColor.b = 0.2 * lColor.r + 0.5 * lColor.g + 0.1 * lColor.b  +40.8/255.0;
                                                 
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_FUGU2=STRINGIFY(
    varying vec2 v_texCoord;
    varying vec4 v_fragmentColor;
    void main()
    {
        vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
        gl_FragColor.r=0.393*v_orColor.r+0.769*v_orColor.g+0.189*v_orColor.b;
        gl_FragColor.g=0.349*v_orColor.r+0.686*v_orColor.g+0.168*v_orColor.b ;
        gl_FragColor.b=0.272*v_orColor.r+0.534*v_orColor.g+0.131*v_orColor.b ;
        gl_FragColor.a=v_orColor.a;
    }
);

static const GLchar* strFSH_GETE=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                           
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                               
        lColor.r = 1.9 * lColor.r - 0.3 * lColor.g -0.2 * lColor.b  -87.0/255.0;
        lColor.g = -0.2 * lColor.r + 1.7 * lColor.g - 0.1 * lColor.b  -87.0/255.0;
        lColor.b = -0.1 * lColor.r - 0.6 * lColor.g + 2.0 * lColor.b  -87.0/255.0;
                                               
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_RUISE=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                           
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                               
        lColor.r = 4.8 * lColor.r -1.0 * lColor.g - 0.1 * lColor.b  -388.4/255.0;
        lColor.g = -0.5 * lColor.r + 4.4 * lColor.g - 0.1 * lColor.b  -388.4/255.0;
        lColor.b = -0.5 * lColor.r -1.0 * lColor.g + 5.2 * lColor.b  -388.4/255.0;
                                               
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_DANYA=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                            
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                                
        lColor.r = 0.6 * lColor.r +0.3 * lColor.g +0.1 * lColor.b  +73.3/255.0;
        lColor.g = 0.2 * lColor.r + 0.7 * lColor.g +0.1 * lColor.b  +73.3/255.0;
        lColor.b = 0.2 * lColor.r +0.3 * lColor.g + 0.4 * lColor.b  +73.3/255.0;
                                                
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_JIUHONG=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                            
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                                
        lColor.r = 1.2 * lColor.r +0.0 * lColor.g +0.0 * lColor.b ;
        lColor.g = 0.0 * lColor.r + 0.9 * lColor.g +0.0 * lColor.b;
        lColor.b = 0.0 * lColor.r +0.0 * lColor.g + 0.8 * lColor.b;
                                                
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_QINGNING=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                            
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                                
        lColor.r = 0.9 * lColor.r +0.0 * lColor.g +0.0 * lColor.b  ;
        lColor.g = 0.0 * lColor.r + 1.1 * lColor.g +0.0 * lColor.b ;
        lColor.b = 0.0 * lColor.r +0.0 * lColor.g + 0.9 * lColor.b ;
                                                
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_LANGMAN=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                            
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                                
        lColor.r = 0.9 * lColor.r +0.0 * lColor.g +0.0 * lColor.b  +63.0/255.0;
        lColor.g = 0.0 * lColor.r + 0.9 * lColor.g +0.0 * lColor.b  +63.0/255.0;
        lColor.b = 0.0 * lColor.r +0.0 * lColor.g + 0.9 * lColor.b  +63.0/255.0;
                                                
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_GUANGYUN=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                              
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                                  
        lColor.r = 0.9 * lColor.r +0.0 * lColor.g +0.0 * lColor.b  +64.9/255.0;
        lColor.g = 0.0 * lColor.r + 0.9 * lColor.g +0.0 * lColor.b  +64.9/255.0;
        lColor.b = 0.0 * lColor.r +0.0 * lColor.g + 0.9 * lColor.b  +64.9/255.0;
                                                  
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_LANDIAO=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                               
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                                   
        lColor.r = 2.1 * lColor.r -1.4 * lColor.g +0.6 * lColor.b  -31.0/255.0;
        lColor.g = -0.3 * lColor.r + 2.0 * lColor.g -0.3 * lColor.b  -31.0/255.0;
        lColor.b = -1.1 * lColor.r -0.2 * lColor.g + 2.6 * lColor.b  -31.0/255.0;
                                                   
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_MENGHUAN=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                              
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                                  
        lColor.r = 0.8 * lColor.r +0.3 * lColor.g +0.1 * lColor.b  +46.5/255.0;
        lColor.g = 0.1 * lColor.r + 0.9 * lColor.g +0.0 * lColor.b  +46.5/255.0;
        lColor.b = 0.1 * lColor.r +0.3 * lColor.g + 0.7 * lColor.b  +46.5/255.0;
                                                  
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_YESE=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
                                               
    void main()
    {
        vec4 lColor=texture2D(CC_Texture0, v_texCoord);
                                                   
        lColor.r = 1.0 * lColor.r +0.0 * lColor.g +0.0 * lColor.b  -66.6/255.0;
        lColor.g = 0.0 * lColor.r +1.1 * lColor.g +0.0 * lColor.b -66.6/255.0;
        lColor.b = 0.0 * lColor.r +0.0 * lColor.g + 1.0 * lColor.b -66.6/255.0;
                                                   
        gl_FragColor = lColor;
    }
);

static const GLchar* strFSH_UKNOWN=STRINGIFY(
   varying vec4 v_fragmentColor;
   varying vec2 v_texCoord;
   void main()
   {
       float T = 2.0;
       vec2 st = v_texCoord.st;
       vec3 irgb = texture2D(CC_Texture0, st).rgb;
       vec3 black = vec3(0.0, 0.0, 0.0);
       gl_FragColor = vec4(mix(black, irgb, T),1.0);
   }
);


static const GLchar* strFSH_DUIBIDU=STRINGIFY(
     varying vec4 v_fragmentColor;
     varying vec2 v_texCoord;
     void main()
     {
         float T = 2.0;
         vec2 st = v_texCoord.st;
         vec3 irgb = texture2D(CC_Texture0, st).rgb;
         vec3 target = vec3(0.5, 0.5, 0.5);
         gl_FragColor = vec4(mix(target, irgb, T), 1.0);
     }
);

static const GLchar* strFSH_BAOHEDU=STRINGIFY(
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    void main()
    {
        float T = 3.0;
        vec2 st = v_texCoord.st;
        vec3 irgb = texture2D(CC_Texture0, st).rgb;
        float luminance = dot(irgb, W);
        vec3 target = vec3(luminance, luminance, luminance);
        gl_FragColor = vec4(mix(target, irgb, T), 1.0);
    }
);


static const GLchar* strFSH_BIANYUANJIANCHE=STRINGIFY(   //边缘检测
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
    const vec2 texOffset = vec2( 0.005, 0.005);
    const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0.0);
    void main()
    {
        vec2 tc0 = v_texCoord.st + vec2(-texOffset.s, -texOffset.t);
        vec2 tc1 = v_texCoord.st + vec2(         0.0, -texOffset.t);
        vec2 tc2 = v_texCoord.st + vec2(+texOffset.s, -texOffset.t);
        vec2 tc3 = v_texCoord.st + vec2(-texOffset.s,          0.0);
        vec2 tc4 = v_texCoord.st + vec2(         0.0,          0.0);
        vec2 tc5 = v_texCoord.st + vec2(+texOffset.s,          0.0);
        vec2 tc6 = v_texCoord.st + vec2(-texOffset.s, +texOffset.t);
        vec2 tc7 = v_texCoord.st + vec2(         0.0, +texOffset.t);
        vec2 tc8 = v_texCoord.st + vec2(+texOffset.s, +texOffset.t);

        vec4 col0 = texture2D(CC_Texture0, tc0);
        vec4 col1 = texture2D(CC_Texture0, tc1);
        vec4 col2 = texture2D(CC_Texture0, tc2);
        vec4 col3 = texture2D(CC_Texture0, tc3);
        vec4 col4 = texture2D(CC_Texture0, tc4);
        vec4 col5 = texture2D(CC_Texture0, tc5);
        vec4 col6 = texture2D(CC_Texture0, tc6);
        vec4 col7 = texture2D(CC_Texture0, tc7);
        vec4 col8 = texture2D(CC_Texture0, tc8);

        vec4 sum = 8.0 * col4 - (col0 + col1 + col2 + col3 + col5 + col6 + col7 + col8);
        gl_FragColor = vec4(sum.rgb, 1.0) * v_fragmentColor;
   }
);

static const GLchar* strFSH_FUDIAO=STRINGIFY(   //浮雕
    varying vec4 v_fragmentColor;
    varying vec2 v_texCoord;
    const vec2 texOffset = vec2( 0.005, 0.005);
    const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0.0);
    void main()
    {
        vec2 tc0 = v_texCoord.st + vec2(-texOffset.s, -texOffset.t);
        vec2 tc1 = v_texCoord.st + vec2(         0.0, -texOffset.t);
        vec2 tc2 = v_texCoord.st + vec2(-texOffset.s,          0.0);
        vec2 tc3 = v_texCoord.st + vec2(+texOffset.s,          0.0);
        vec2 tc4 = v_texCoord.st + vec2(         0.0, +texOffset.t);
        vec2 tc5 = v_texCoord.st + vec2(+texOffset.s, +texOffset.t);

        vec4 col0 = texture2D(CC_Texture0, tc0);
        vec4 col1 = texture2D(CC_Texture0, tc1);
        vec4 col2 = texture2D(CC_Texture0, tc2);
        vec4 col3 = texture2D(CC_Texture0, tc3);
        vec4 col4 = texture2D(CC_Texture0, tc4);
        vec4 col5 = texture2D(CC_Texture0, tc5);
        vec4 sum = vec4(0.5) + (col0 + col1 + col2) - (col3 + col4 + col5);
        float lum = dot(sum, lumcoeff);
        gl_FragColor = vec4(lum, lum, lum, 1.0) * v_fragmentColor;
    }
);

static const GLchar* strFSH_MOTION_BLUR =STRINGIFY(   //MotionBlur
   varying vec2 v_texCoord;
   varying vec4 v_fragmentColor;
   const float contrast = 1.6;
   const float brightness = 0.3;
   const float factor = -0.5 * contrast + brightness;
   void main()
  {
    gl_FragColor = vec4(0.0);
//vec4 c = texture2D(u_Texture,v_texCoord.st);
//gl_FragColor = clamp(c * contrast + factor, 0.0, 1.);
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0, -0.014))*0.0044299121055113265;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0, -0.012))*0.00895781211794;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0, -0.010))*0.0215963866053;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0, -0.008))*0.0443683338718;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0, -0.006))*0.0776744219933;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0, -0.004))*0.115876621105;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0, -0.002))*0.147308056121;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord         )*0.159576912161;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0,  0.002))*0.147308056121;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0,  0.004))*0.115876621105;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0,  0.006))*0.0776744219933;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0,  0.008))*0.0443683338718;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0,  0.010))*0.0215963866053;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0,  0.012))*0.00895781211794;
    gl_FragColor += texture2D(CC_Texture0, v_texCoord + vec2(0.0,  0.014))*0.0044299121055113265;
  }
);

static const GLchar* strFSH_1977 =STRINGIFY
(   //1977
    varying vec2 v_texCoord;
    varying vec4 v_fragmentColor;
    void main()
    {
        vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
        float r= texture2D(CC_Texture1, vec2(v_orColor.r, 0.16666)).r;
        float g=texture2D(CC_Texture1, vec2(v_orColor.g, 0.5)).g;
        float b=texture2D(CC_Texture1, vec2(v_orColor.b, 0.83333)).b;
        gl_FragColor=vec4(r,g,b,v_orColor.a);
    }
 );

static const GLchar* strFSH_KELVIN =STRINGIFY
(   //Kelvin
 varying vec2 v_texCoord;
 varying vec4 v_fragmentColor;
 void main()
 {
     vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
     
     vec2 lookup;
     lookup.y=0.5;
     
     lookup.x=v_orColor.r;
     float r= texture2D(CC_Texture1,lookup).r;
     
     lookup.x = v_orColor.g;
     float g=texture2D(CC_Texture1, lookup).g;
     
     lookup.x = v_orColor.b;
     float b=texture2D(CC_Texture1,lookup).b;

     gl_FragColor=vec4(r,g,b,v_orColor.a);
 }
 );

static const GLchar* strFSH_NASHVILLE =STRINGIFY
(   //Nashville
 varying vec2 v_texCoord;
 varying vec4 v_fragmentColor;
 void main()
 {
     vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
     float r= texture2D(CC_Texture1, vec2(v_orColor.r, 0.16666)).r;
     float g=texture2D(CC_Texture1, vec2(v_orColor.g, 0.5)).g;
     float b=texture2D(CC_Texture1, vec2(v_orColor.b, 0.83333)).b;
     gl_FragColor=vec4(r,g,b,v_orColor.a);
 }
 );

static const GLchar* strFSH_VALENCIAL =STRINGIFY
(   //VALENCIAL
 varying vec2 v_texCoord;
 varying vec4 v_fragmentColor;
 mat3 saturateMatrix = mat3(
                            1.1402,
                            -0.0598,
                            -0.061,
                            -0.1174,
                            1.0826,
                            -0.1186,
                            -0.0228,
                            -0.0228,
                            1.1772);
 
 vec3 lumaCoeffs = vec3(.3, .59, .11);
 void main()
 {
     vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
     float r= texture2D(CC_Texture1, vec2(v_orColor.r, 0.16666)).r;
     float g=texture2D(CC_Texture1, vec2(v_orColor.g, 0.5)).g;
     float b=texture2D(CC_Texture1, vec2(v_orColor.b, 0.83333)).b;
     
     vec3 color=saturateMatrix * vec3(r,g,b);
     /*
     float luma = dot(lumaCoeffs, color);
     color = vec3(
                  texture2D(CC_Texture2, vec2(luma, color.r)).r,
                  texture2D(CC_Texture2, vec2(luma, color.g)).g,
                  texture2D(CC_Texture2, vec2(luma, color.b)).b);
      */
     
     gl_FragColor=vec4(color,v_orColor.a);
 }
 );

//
//static const GLchar* strFSH_WALDEN =STRINGIFY
//(   //Nashville
// varying vec2 v_texCoord;
// varying vec4 v_fragmentColor;
// void main()
// {
//     vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
//     float r= texture2D(CC_Texture1, vec2(v_orColor.r, 0.16666)).r;
//     float g=texture2D(CC_Texture1, vec2(v_orColor.g, 0.5)).g;
//     float b=texture2D(CC_Texture1, vec2(v_orColor.b, 0.83333)).b;
//     gl_FragColor=vec4(r,g,b,v_orColor.a);
// }
// );
//
//static const GLchar* strFSH_HEFE =STRINGIFY
//(   //Nashville
// varying vec2 v_texCoord;
// varying vec4 v_fragmentColor;
// void main()
// {
//     vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
//     float r= texture2D(CC_Texture1, vec2(v_orColor.r, 0.16666)).r;
//     float g=texture2D(CC_Texture1, vec2(v_orColor.g, 0.5)).g;
//     float b=texture2D(CC_Texture1, vec2(v_orColor.b, 0.83333)).b;
//     gl_FragColor=vec4(r,g,b,v_orColor.a);
// }
// );
//
//static const GLchar* strFSH_INKWELL =STRINGIFY
//(   //Nashville
// varying vec2 v_texCoord;
// varying vec4 v_fragmentColor;
// void main()
// {
//     vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
//     float r= texture2D(CC_Texture1, vec2(v_orColor.r, 0.16666)).r;
//     float g=texture2D(CC_Texture1, vec2(v_orColor.g, 0.5)).g;
//     float b=texture2D(CC_Texture1, vec2(v_orColor.b, 0.83333)).b;
//     gl_FragColor=vec4(r,g,b,v_orColor.a);
// }
// );


static const GLchar* strFSH_OVERLAY =STRINGIFY(   //OverLay
     varying vec2 v_texCoord;
     varying vec4 v_fragmentColor;
    float getValue2(float baseValue,float mixedValue)
    {
        float lValue=0.0;
    
        if(baseValue<=0.5)
        {
            lValue=2.0*baseValue*mixedValue;
        }
        else
        {
            lValue=1.0-2.0*(1.0-baseValue)*(1.0-mixedValue);
        }
    
        return lValue;
    }
     vec4 changeColor(vec4 baseColor,vec4 mixedColor)
     {
         vec4 lTargetColor;
    
         lTargetColor.r=getValue2(baseColor.r,mixedColor.r);
         lTargetColor.g=getValue2(baseColor.g,mixedColor.g);
         lTargetColor.b=getValue2(baseColor.b,mixedColor.b);
    
         lTargetColor.r=lTargetColor.r*mixedColor.a;
         lTargetColor.g=lTargetColor.g*mixedColor.a;
         lTargetColor.b=lTargetColor.b*mixedColor.a;
    
         if(baseColor.a==0.0)
         {
             lTargetColor.a=0.0;
         }
         else
         {
            lTargetColor.a=mixedColor.a;
         }
    
         return lTargetColor;
     }
     void main()
     {
         vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
         vec4 v_tColor=texture2D(CC_Texture1, v_texCoord);
         gl_FragColor = changeColor(v_tColor,v_orColor);
     }
);

static const GLchar* strFSH_OVERLAY2 =STRINGIFY
(   //OverLay
 
    varying vec2 v_texCoord;
    varying vec4 v_fragmentColor;
    float getValue2(float baseValue,float mixedValue)
    {
        float lValue=0.0;
                                                   
        if(baseValue<=0.5)
        {
             lValue=2.0*baseValue*mixedValue;
        }
        else
        {
             lValue=1.0-2.0*(1.0-baseValue)*(1.0-mixedValue);
        }
                                                   
         return lValue;
    }
    vec4 changeColor(vec4 baseColor,vec4 mixedColor)
    {
        vec4 lTargetColor;
                                                   
        lTargetColor.r=getValue2(baseColor.r,mixedColor.r);
        lTargetColor.g=getValue2(baseColor.g,mixedColor.g);
        lTargetColor.b=getValue2(baseColor.b,mixedColor.b);
                                                   
        lTargetColor.r=lTargetColor.r*mixedColor.a;
        lTargetColor.g=lTargetColor.g*mixedColor.a;
        lTargetColor.b=lTargetColor.b*mixedColor.a;
        
        
        lTargetColor.a=mixedColor.a;
        
        return lTargetColor;
    }
    void main()
    {
        vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
        vec4 v_tColor=texture2D(CC_Texture1, v_texCoord);
       //  gl_FragColor = changeColor(v_orColor,v_tColor);
         gl_FragColor = changeColor(v_tColor,v_orColor);
     }
);

static const GLchar* strFSH_ROUGUANG =STRINGIFY(   //柔光
    varying vec2 v_texCoord;
    varying vec4 v_fragmentColor;
    float getValue(float baseValue,float mixedValue)
    {
        float lValue;
        if(baseValue<0.5)
        {
            lValue=2.0*(mixedValue*0.5+64.0/255.0)*baseValue;
        }
        else
        {
            lValue=1.0-2.0*(1.0-(mixedValue*0.5+64.0/255.0))*(1.0-baseValue);
        }
        return lValue;
    }
    void main()
    {
        vec4 v_orColor = texture2D(CC_Texture0, v_texCoord);
        vec4 v_tColor=texture2D(CC_Texture1, v_texCoord);
        gl_FragColor.r=getValue(v_orColor.r,v_tColor.r);
        gl_FragColor.g=getValue(v_orColor.g,v_tColor.g);
        gl_FragColor.b=getValue(v_orColor.b,v_tColor.b);
        gl_FragColor.a=v_orColor.a;
    }
);

#pragma mark - New1.1


static const GLchar* strFSH_Crayon=STRINGIFY(
                                             varying vec4 v_fragmentColor;
                                             varying vec2 v_texCoord;
                                             
                                             uniform sampler2D outline;
                                             uniform sampler2D preprocessed;
                                             uniform sampler2D noise;
                                             uniform sampler2D hatch;
                                             uniform sampler2D background;
                                             uniform sampler2D tex;
                                             
                                             
                                             float getValue2(float baseValue,float mixedValue)
                                             {
                                                 float lValue=0.0;
                                                 
                                                 
                                                 if(baseValue<=0.5)
                                                 {
                                                     lValue=2.0*baseValue*mixedValue;
                                                 }
                                                 else
                                                 {
                                                     lValue=1.0-2.0*(1.0-baseValue)*(1.0-mixedValue);
                                                 }
                                                 
                                                 return lValue;
                                             }
                                             
                                             void main()
                                             {
                                                 vec4 texColor=texture2D(tex,v_texCoord);
                                                 vec2 changeXY_vec=vec2(v_texCoord.y,v_texCoord.x);
                                                 vec4 hatchColor=texture2D(hatch,changeXY_vec);
                                                 vec4 backgroundColor=texture2D(background,v_texCoord);
                                                 vec3 rgb;
                                                 
                                                 hatchColor.rgb=hatchColor.rgb*0.7;
                                                 
                                                 
                                                 texColor.r=getValue2(texColor.r,hatchColor.r);
                                                 texColor.g=getValue2(texColor.g,hatchColor.g);
                                                 texColor.b=getValue2(texColor.b,hatchColor.b);
                                                 texColor.a=texColor.a;
                                                 
                                                 texColor.r=texColor.r*0.7+backgroundColor.r*0.3;
                                                 texColor.g=texColor.g*0.7+backgroundColor.g*0.3;
                                                 texColor.b=texColor.b*0.7+backgroundColor.b*0.3;
                                                 
                                                 gl_FragColor=texColor;
                                                 
                                             }
                                             
);

static const GLchar* strFSH_Pencil=STRINGIFY(
                                             varying vec4 v_fragmentColor;
                                             varying vec2 v_texCoord;
                                             
                                             
                                             uniform sampler2D outline;
                                             uniform sampler2D preprocessed;
                                             uniform sampler2D noise;
                                             uniform sampler2D hatch;
                                             uniform sampler2D background;
                                             uniform sampler2D tex;
                                             
                                             float getValue2(float mixedValue,float baseValue)
                                             {
                                                 float lValue;
                                                 if(baseValue<=0.5)
                                                 {
                                                     lValue=2.0*mixedValue*baseValue+mixedValue*mixedValue*(1.0-2.0*baseValue);
                                                 }
                                                 else
                                                 {
                                                     lValue=2.0*mixedValue*(1.0-baseValue)+sqrt(mixedValue)*(2.0*baseValue-1.0);
                                                 }
                                                 return lValue;
                                             }
                                             
                                             
                                             
                                             void main()
                                             {
                                                 vec4 texColor=texture2D(tex,v_texCoord);
                                                 vec4 hatchColor=texture2D(hatch,v_texCoord);
                                                 vec4 noiseColor=texture2D(noise,v_texCoord);
                                                 vec4 backgroundColor=texture2D(background,v_texCoord);
                                                 
                                                 // hatchColor=mix(hatchColor,noiseColor,0.5);
                                                 
                                                 
                                                 
                                                 texColor.r=getValue2(texColor.r,hatchColor.r);
                                                 texColor.g=getValue2(texColor.g,hatchColor.g);
                                                 texColor.b=getValue2(texColor.b,hatchColor.b);
                                                 
                                                 gl_FragColor=texColor;
                                             }
                                             
                                             
);

static const GLchar* strFSH_WaterColor=STRINGIFY(
                                                 varying vec4 v_fragmentColor;
                                                 varying vec2 v_texCoord;
                                                 
                                                 
                                                 uniform sampler2D outline;
                                                 uniform sampler2D preprocessed;
                                                 uniform sampler2D noise;
                                                 uniform sampler2D hatch;
                                                 uniform sampler2D background;
                                                 uniform sampler2D tex;
                                                 
                                                 float getValue2(float mixedValue,float baseValue)
                                                 {
                                                     float lValue;
                                                     if(baseValue<=0.5)
                                                     {
                                                         lValue=2.0*mixedValue*baseValue+mixedValue*mixedValue*(1.0-2.0*baseValue);
                                                     }
                                                     else
                                                     {
                                                         lValue=2.0*mixedValue*(1.0-baseValue)+sqrt(mixedValue)*(2.0*baseValue-1.0);
                                                     }
                                                     return lValue;
                                                 }

                                                 
                                                 
                                                 void main()
                                                 {
                                                     vec4 texColor=texture2D(tex,v_texCoord);
                                                     vec4 hatchColor=texture2D(hatch,v_texCoord);
                                                     vec4 noiseColor=texture2D(noise,v_texCoord);
                                                     vec4 backgroundColor=texture2D(background,v_texCoord);
                                                     
                                                     // hatchColor=mix(hatchColor,noiseColor,0.5);
                                                     float alpha=0.45;
                                                     
                                                     hatchColor.r=hatchColor.r*alpha+texColor.r*(1.0-alpha);
                                                     hatchColor.g=hatchColor.b*alpha+texColor.g*(1.0-alpha);
                                                     hatchColor.b=hatchColor.b*alpha+texColor.b*(1.0-alpha);
                                                     
                                                     
                                                     texColor.r=getValue2(texColor.r,hatchColor.r);
                                                     texColor.g=getValue2(texColor.g,hatchColor.g);
                                                     texColor.b=getValue2(texColor.b,hatchColor.b);
                                                     
                                                     gl_FragColor=texColor;
                                                 }
                                                 
);


static const GLchar* strFSH_GaussianBlur =STRINGIFY(   //高斯模糊
                                                    varying vec2 v_texCoord;
                                                    varying vec4 v_fragmentColor;
                                                    
                                                    //    float weight[3][3]={
                                                    //        0.0947,
                                                    //    }
                                                    
                                                    float weight_0=0.05;
                                                    float weight_1=0.1;
                                                    float weight_2=0.4;
                                                    
                                                    
                                                    vec3 blur(vec2 p)
                                                    {
                                                        vec3 col = vec3(0);
                                                        // float r=1.0;
                                                        float dis=0.008;
                                                        for(float i=-1.0;i<=1.0;i++)
                                                        {
                                                            for(float j=-1.0;j<=1.0;j++)
                                                            {
                                                                if(abs(i)+abs(j)==0.0)
                                                                {
                                                                    col += texture2D(CC_Texture0,p+vec2(i*dis, j*dis)).rgb * weight_2;
                                                                }
                                                                else if(abs(i)+abs(j)==1.0)
                                                                {
                                                                    col += texture2D(CC_Texture0,p+vec2(i*dis, j*dis)).rgb * weight_1;
                                                                }
                                                                else if(abs(i)+abs(j)==2.0)
                                                                {
                                                                    col += texture2D(CC_Texture0,p+vec2(i*dis, j*dis)).rgb * weight_0;
                                                                }
                                                                
                                                                // col += texture2D(CC_Texture0,p+vec2(i*dis, j*dis)).rgb;
                                                            }
                                                        }
                                                        return col;
                                                        
                                                    }
                                                    
                                                    

                                                    
                                                    void main()
                                                    {
                                                        vec3 lColor = blur(v_texCoord);
                                                        vec4 texColor = texture2D(CC_Texture0, v_texCoord);
                                                        
                                                        
                                                        float alpha=0.46;
                                                        lColor.r=texColor.r*alpha+lColor.r*(1.0-alpha);
                                                        lColor.g=texColor.b*alpha+lColor.g*(1.0-alpha);
                                                        lColor.b=texColor.b*alpha+lColor.b*(1.0-alpha);
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        vec4 XColor;
                                                        XColor.rgb=lColor;
                                                        XColor.a=1.0;
                                                        
                                                        gl_FragColor=XColor;
                                                        
                                                        
                                                        
                                                        // gl_FragColor = vec4(col, 1.0) * v_fragmentColor;
                                                        
                                                        //gl_FragColor.a=v_orColor.a;
                                                        
                                                        
                                                        
                                                    }
);


static const GLchar* strFSH_Shiny=STRINGIFY(   //浮雕
                                            varying vec4 v_fragmentColor;
                                            varying vec2 v_texCoord;
                                            
                                            uniform sampler2D outline;
                                            uniform sampler2D preprocessed;
                                            uniform sampler2D noise;
                                            uniform sampler2D hatch;
                                            uniform sampler2D background;
                                            uniform sampler2D tex;
                                            
                                            //const vec2 texOffset = vec2( 0.005, 0.005);
                                            const vec2 texOffset = vec2( 0.0015, 0.0015);
                                            const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0.0);
                                            
                                            float getValue2(float baseValue,float mixedValue)
                                            {
                                                float lValue=0.0;
                                                
                                                if(baseValue<=0.5)
                                                {
                                                    lValue=2.0*baseValue*mixedValue;
                                                }
                                                else
                                                {
                                                    lValue=1.0-2.0*(1.0-baseValue)*(1.0-mixedValue);
                                                }
                                                
                                                
                                                
                                                return lValue;
                                            }
                                            
                                            
                                            float getValue(float baseValue,float mixedValue)
                                            {
                                                float lValue=0.0;
                                                lValue=baseValue*mixedValue;
                                                lValue=lValue*0.5+baseValue*0.5;
                                                return lValue;
                                            }
                                            
                                            float getValue3(float baseValue,float mixedValue)
                                            {
                                                float lValue;
                                                if(mixedValue==1.0)
                                                {
                                                    lValue=mixedValue;
                                                }
                                                else
                                                {
                                                    lValue=min(1.0,baseValue/(1.0-mixedValue));
                                                }
                                                return lValue;
                                            }
                                            
                                            void main()
                                            {
                                                vec4 hatchColor=texture2D(hatch,v_texCoord);
                                                vec4 noiseColor=texture2D(noise,v_texCoord);
                                                
                                                vec2 tc0 = v_texCoord.st + vec2(-texOffset.s, -texOffset.t);
                                                vec2 tc1 = v_texCoord.st + vec2(         0.0, -texOffset.t);
                                                vec2 tc2 = v_texCoord.st + vec2(-texOffset.s,          0.0);
                                                vec2 tc3 = v_texCoord.st + vec2(+texOffset.s,          0.0);
                                                vec2 tc4 = v_texCoord.st + vec2(         0.0, +texOffset.t);
                                                vec2 tc5 = v_texCoord.st + vec2(+texOffset.s, +texOffset.t);
                                                
                                                vec4 col0 = texture2D(tex, tc0);
                                                vec4 col1 = texture2D(tex, tc1);
                                                vec4 col2 = texture2D(tex, tc2);
                                                vec4 col3 = texture2D(tex, tc3);
                                                vec4 col4 = texture2D(tex, tc4);
                                                vec4 col5 = texture2D(tex, tc5);
                                                vec4 lColor=texture2D(tex,v_texCoord);
                                                lColor.r=getValue2(lColor.r,hatchColor.r);
                                                lColor.g=getValue2(lColor.g,hatchColor.g);
                                                lColor.b=getValue2(lColor.b,hatchColor.b);
                                                
                                                
                                                
                                                
                                                lColor=lColor+ (col3 + col4 + col5)-(col0 + col1 + col2) ;
                                                
                                                //        lColor.r=getValue3(lColor.r,noiseColor.r);
                                                //        lColor.g=getValue3(lColor.g,noiseColor.g);
                                                //        lColor.b=getValue3(lColor.b,noiseColor.b);
                                                
                                                
                                                gl_FragColor=lColor;
                                            }
                                            );

static const GLchar* strFSH_NewOverLay=STRINGIFY(
                                                 varying vec4 v_fragmentColor;
                                                 varying vec2 v_texCoord;
                                                 
                                                 uniform sampler2D noise;
                                                 uniform sampler2D hatch;
                                                 uniform sampler2D background;
                                                 uniform sampler2D tex;
                                                 
                                                 float getValue2(float baseValue,float mixedValue)
                                                 {
                                                     float lValue=0.0;
                                                     
                                                     if(baseValue<=0.5)
                                                     {
                                                         lValue=2.0*baseValue*mixedValue;
                                                     }
                                                     else
                                                     {
                                                         lValue=1.0-2.0*(1.0-baseValue)*(1.0-mixedValue);
                                                     }
                                                     
                                                     return lValue;
                                                 }
                                                 
                                                 
                                                 
                                                 void main()
                                                 {
                                                     vec4 texColor=texture2D(tex,v_texCoord);
                                                     vec4 hatchColor=texture2D(hatch,v_texCoord);
                                                     vec4 backgroundColor=texture2D(background,v_texCoord);
                                                     vec3 rgb;
                                                     
                                                     hatchColor.r=getValue2(texColor.r,hatchColor.r);
                                                     hatchColor.g=getValue2(texColor.g,hatchColor.g);
                                                     hatchColor.b=getValue2(texColor.b,hatchColor.b);
                                                     
                                                     float alpha=hatchColor.a;
                                                     
                                                     texColor.r=hatchColor.r*alpha+texColor.r*(1.0-alpha);
                                                     texColor.g=hatchColor.b*alpha+texColor.g*(1.0-alpha);
                                                     texColor.b=hatchColor.b*alpha+texColor.b*(1.0-alpha);
                                                     
                                                     gl_FragColor=texColor;

                                                     
                                                 }
);


#endif
