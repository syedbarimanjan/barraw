#include <stdlib.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

typedef struct {
  int id;
  float x;
  float y;
  float width;
  float height;
  Color color;
} Rectangled;

typedef struct {
  int id;
  int centerX;
  int centerY;
  float radius;
  Color color;
} Circle;


typedef struct {
  int id;
  int startPosX;
  int startPosY; 
  int endPosX;
  int endPosY;
  Color color;
} Line;

typedef struct {
  int id;
  Circle *circles;
} FreeLine;

typedef enum {
  SHAPE_RECTANGLE,
  SHAPE_CIRCLE,
  SHAPE_LINE,
  SHAPE_FREELINE
} Shapes;

typedef struct {
  Shapes type;
  union {
    Rectangled rectangle;
    Circle circle;
    Line line;
    FreeLine freeline;
  } shape;
} Shape;

// can use flexible array method here.
typedef struct {
  int id;
  Shape *shapesArray;
  int shapeCount;
} Layer;

typedef enum Tools {
  PEN,
  LINE,
  RECTANGLE,
  CIRCLE,
} Tools;

Tools tools;

int main() {
  InitWindow(2000, 1000, "Barraw");
  bool mouseWasPressed = false;
  bool mouseIsDown = false;

  int shapesCount;
  Shape *shapes;
  shapes = malloc(sizeof(Shape) * shapesCount);

  
  RenderTexture2D target = LoadRenderTexture(2000,1000);
  RenderTexture2D target2 = LoadRenderTexture(2000,1000);
  
  Vector2 prevMouse = {0};
  bool penStarted = false;
  Vector2 pm = {0};
  
  float penradius = 1.0f;
  tools= PEN;
  while(!WindowShouldClose()) {
      
    // tools = CIRCLE;
    if(IsKeyPressed(KEY_R)){
      tools = RECTANGLE;
    } else if(IsKeyPressed(KEY_C)){
      tools = CIRCLE;
    } else if(IsKeyPressed(KEY_P)){
      tools = PEN;
    } else if (IsKeyPressed(KEY_L)){
      tools = LINE;
    }

    if(IsKeyPressed(KEY_A)){
      free(shapes);
    }

    
    Vector2 curr = GetMousePosition();
    float mousewheel = GetMouseWheelMove();
    penradius+=GetMouseWheelMove();
    if (penradius < 2) penradius = 2;
    if (penradius > 50) penradius = 50;
    
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && tools == PEN)
    {
      if (!penStarted)
      {
        prevMouse = curr;
        penStarted = true;
      }
      
      

      float dx = curr.x - prevMouse.x;
      float dy = curr.y - prevMouse.y;
      float dist = sqrt(dx*dx + dy*dy);

      float spacing = 0.1f;

      BeginTextureMode(target);

      for (float i = 0; i < dist; i += spacing)
      {
          float t = i / dist;

          float x = prevMouse.x + dx * t;
          float y = prevMouse.y + dy * t;

          DrawCircle(x, y, penradius, RED);
        }
        
        DrawRectangle(10,10,100,100,BLACK);
  
        DrawText(TextFormat("%d a",curr),10,10,20,RED);
        DrawText(TextFormat("%d bv",prevMouse),20,30,20,RED);
      EndTextureMode();
      
      prevMouse = curr;
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && tools == PEN)
    {
        penStarted = false;
    }

    Vector2 a;
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tools == LINE) {
      if(!mouseIsDown){
        a=GetMousePosition();
        pm = a;
        mouseIsDown = true;
        continue;
      }
      
      
      a=GetMousePosition();
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target2);
      ClearBackground(BLACK);

      DrawLine(pm.x,pm.y,a.x,a.y,GREEN);

      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == LINE) {
      mouseIsDown = false;
      shapesCount+=1;
      shapes = realloc(shapes,sizeof(Shape) * shapesCount);
      shapes->type = SHAPE_LINE;
      shapes[0].shape.line = (Line) {
        .color = GREEN,
        .id = 1,
        .startPosX = pm.x,
        .startPosY = pm.y,
        .endPosX = a.x,
        .endPosY = a.y,
      };
      // BeginTextureMode(target);
      // DrawLine(pm.x,pm.y,a.x,a.y,GREEN);
      // DrawRectangle(10,10,100,100,BLACK);
      // DrawFPS(10,10);
      // EndTextureMode();
      
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tools == RECTANGLE) {
      if(!mouseIsDown){
        a=GetMousePosition();
        pm = a;
        mouseIsDown = true;
        continue;
      }
      
      
      a=GetMousePosition();
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target2);
      ClearBackground(BLACK);
      DrawRectangleLines(pm.x,pm.y,a.x-pm.x,a.y-pm.y,BLUE);
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
    }
    else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == RECTANGLE){

      mouseIsDown = false;
      BeginTextureMode(target);
      DrawRectangleLines(pm.x,pm.y,a.x-pm.x,a.y-pm.y,BLUE);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
    }

    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tools == CIRCLE) {
      if(!mouseIsDown){
        a=GetMousePosition();
        pm = a;
        mouseIsDown = true;
        continue;
      }
      
      
      a=GetMousePosition();
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target2);
      ClearBackground(BLACK);
      float dx = a.x-pm.x;
      float dy = a.y -pm.y;
      float radius = sqrt(dx*dx + dy*dy);
      
      DrawCircleLines((pm.x+a.x)/2,(pm.y+a.y)/2,radius/2,PINK);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == CIRCLE) {
      mouseIsDown = false;
      int x= GetMouseX();
      int y= GetMouseY();
      BeginTextureMode(target);
      float dx = a.x-pm.x;
      float dy = a.y -pm.y;
      float radius = sqrt(dx*dx + dy*dy);
      
      DrawCircleLines((pm.x+a.x)/2,(pm.y+a.y)/2,radius/2,PINK);
      
      DrawRectangle(10,10,100,100,BLACK);
      DrawFPS(10,10);
      EndTextureMode();
      
    }
    
    BeginTextureMode(target);
      for(int i = 0; i < shapesCount; i++){
        if(shapes->type == SHAPE_LINE) {
          DrawLine(shapes[i].shape.line.startPosX,shapes[i].shape.line.startPosY,shapes[i].shape.line.endPosX,shapes[i].shape.line.endPosY,shapes[i].shape.line.color);
        }
      }
    EndTextureMode();
    BeginDrawing();
    // ClearBackground(RAYWHITE);
    DrawTextureRec(target2.texture, (Rectangle){ 0, 0, (float)target2.texture.width, (float)-target2.texture.height }, (Vector2) { 0, 0 }, WHITE);
    DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
    EndDrawing();
  }
    
  CloseWindow();
    
  return 0;
}