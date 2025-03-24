#ifndef THINGS_H
#define THINGS_H

#include"include\raylib.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//state tracks what tool is active
enum State {normal,rect,ellipse,line,HollowRect,HollowEllipse,Bucket,eyedropper,moveref};
//window tracks what window is active
enum Window {paint,super};
enum Brush {circle,square,spray};
typedef struct{
    int thickness;
    Color color;
    enum Brush brush;
}Pen;

typedef struct {
    Color color;
} pixel;

typedef struct
{
    Vector2 top_left;
    Vector2 dim;
    void (*func)(void* a,void*b,void* c,void* d);
    char* texture_path;
    char* ptexture_path;
    bool ispressed;
    bool holdable;
    Texture2D texture;
    Texture2D ptexture;
}button;

typedef struct{
    Vector2 pos;
    float slide;
    Vector2 bar_dim; // width then height
    Color color;
    Vector2 rect_dim;
}slider;

typedef struct{
    char* text;
    int len;
    Vector2 top_left;
    Vector2 dim;
}textbox;

typedef struct{
    int screenwidth;
    int screenheight;

    Vector2 CanvasCenter;
    Vector2 CanvasTopLeft;  // both of these are outside the canvas
    Vector2 CanvasBottomRight;
    int CanvasBorderThickness;

    button* buttons;
    int num_buttons;

    slider* sliders;
    int num_sliders;

    textbox* textboxes;
    int num_textboxes;
    
}Gui;

typedef struct{
    Vector2* arr;
    int len;
    int cap;
}vector;

typedef struct{
    int segments;
    float radius;
    float start_angle;
    float end_angle;
    vector points;
}Ring;

typedef struct{
    Ring* arr;
    int len;
    int cap;

}VectorRing;

typedef struct{
    float angle;
    float radius;
    float distance;
    float speed;
}Player;

typedef struct{
    Vector2 pos1;
    Vector2 pos2;
    int r1;
    int r2;
    int thick;
    Color color;
}Sym;




// pixel.c
void MyDrawRectangle(pixel* screen,Vector2 top_left,Vector2 bottom_right,Color color);
void DrawHollowRectangle(pixel* screen,Vector2 top_left,Vector2 bottom_right,int thickness,Color color);
void MyDrawSymRectangle(pixel* screen,Vector2 top_left,Vector2 bottom_right,Color color);
void MyDrawPixelCanvas(pixel* screen,Vector2 pos,Color color);
bool InCanvas(Vector2 pos); 
void ClearScreen(pixel* screen,vector* qsplines,vector* lines,vector* points);
void DrawScreen(pixel* screen);
void AddGuiBase(pixel* screen,Color color);
void DrawQSplines(vector* qsplines);
void DrawLines(vector* lines);
void ClearCanvas(pixel* screen,vector* qsplines,vector* lines,vector* points);
void ClearCanvasPixels(pixel* screen);
void AddQSplines(pixel* screen,vector* qsplines);
void AddLines(pixel* screen,vector* lines);
void MyDrawCircle(pixel* screen,Vector2 center,int radius,Color color);
void CanvasFitDim(pixel* screen,int width,int height);
void Draw_Textboxes(textbox t);
void MyDrawEllipse(pixel* screen,Vector2 top_left,Vector2 bottom_right,Color color);
void DrawHollowEllipse(pixel* screen,Vector2 top_left,Vector2 bottom_right,int thickness,Color color);
void MyDrawSymEllipse(pixel* screen,Vector2 top_left,Vector2 bottom_right,Color color);
void DrawSymHollowEllipse(pixel* screen,Vector2 top_left,Vector2 bottom_right,int thickness,Color color);
void DrawSymLine(Vector2 pos);
bool IsSymOn();
Vector2 GetSymPoint(Vector2 p);
void ClearMask(pixel* mask);
void DrawSymHollowRectangle(pixel* screen,Vector2 top_left,Vector2 bottom_right,int thickness,Color color);
void DrawObliqueRect(pixel* screen,Vector2 a,Vector2 b ,Vector2 c);
void DrawLabels();


//vector.c
vector GiveVector();
void Vector_Grow50(vector* vec,int index);
void Vector_Add50(vector* vec,Vector2 point,int index);
void Vector_Add50e(vector* vec,Vector2 point);
void Vector_Grow(vector* vec,int index);
void Vector_Add(vector* vec,Vector2 point,int index);
Vector2 Vector_pop(vector* vec);
void Vector_Empty(vector* vec);

VectorRing GiveVectorRing();
void VectorRing_Grow50(VectorRing* vec,int index);
void VectorRing_Add50(VectorRing* vec,Ring point,int index);
void VectorRing_Add50e(VectorRing* vec,Ring point);
void VectorRing_Grow(VectorRing* vec,int index);
void VectorRing_Add(VectorRing* vec,Ring point,int index);
Ring VectorRing_pop(VectorRing* vec);
void VectorRing_Empty(VectorRing* vec);
void VectorRing_Insert(VectorRing* vec,Ring ring,int index);



//buttons.c
Vector2 Vector2Sum(Vector2 a,Vector2 b);
Vector2 Vector2Minus(Vector2 a,Vector2 b);
Vector2 Vector2Mult(float a,Vector2 b);
float Vector2Dot(Vector2 a,Vector2 b);
void Button_LoadTextures();
void Button_UnloadTextures();
void Button_Ifpressed(button but,Vector2 cur_pos,void* a,void* b,void* c,void* d);
void ScreenClearButton(void* a,void* b,void* c,void* d);
void SaveAsPpm6(void* a,void* b,void* c,void* d);
void DrawSlider(slider s);
void LoadPpm6(void* a,void* b,void* c,void* d);
void RectTool(void* a,void* b,void* c,void* d);
void NewButton(void* a,void* b,void* c,void* d);
void EllipseTool(void* a,void* b,void* c,void* d);
void LineTool(void* a,void* b,void* c,void* d);
void RatioLock(void* a,void* b,void* c,void* d);
void Bored(void* a,void* b,void* c,void* d);
void Eraser(void* a,void* b,void* c,void* d);
void HollowRectTool(void* a,void* b,void* c,void* d);
void FillBucket(pixel* screen,Vector2 t);
void FillToolActivation(void* a,void* b,void* c,void* d);
void HollowEllipseTool(void* a,void* b,void* c,void* d);
void FillMaskBucket(pixel* screen,pixel* mask,Vector2 t);
float Vector2Len(Vector2 a);
float Vector2Lensq(Vector2 a);
void EyeDropper(void* a,void* b,void* c,void*d);
void LoadReference(void* a,void* b,void* c,void* d);
void MoveReference(void* a,void* b,void* c,void* d);
void ChangeBrushCircle(void*a , void* b,void* c,void* d);
void ChangeBrushSquare(void*a , void* b,void* c,void* d);
void ChangeBrushSpray(void*a , void* b,void* c,void* d);

#endif