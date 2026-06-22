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
  int circlesCount;
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

  int shapesCount = 0;
  Shape *shapes;
  shapes = malloc(sizeof(Shape) * shapesCount);

  int circleCount = 1000;
  Circle *circles;
  circles = malloc(sizeof(Circle) * circleCount);

  
  RenderTexture2D target = LoadRenderTexture(2000,1000);
  RenderTexture2D target2 = LoadRenderTexture(2000,1000);
  
  Vector2 prevMouse = {0};
  bool penStarted = false;
  Vector2 pm = {0};
  Vector2 a;
  
  float penradius = 1.0f;
  tools= PEN;
  while(!WindowShouldClose()) {

    if(IsKeyPressed(KEY_R)){
      tools = RECTANGLE;
    } else if(IsKeyPressed(KEY_C)){
      tools = CIRCLE;
    } else if(IsKeyPressed(KEY_P)){
      tools = PEN;
    } else if (IsKeyPressed(KEY_L)){
      tools = LINE;
    }

    Vector2 curr = GetMousePosition();
    float mousewheel = GetMouseWheelMove();
    penradius+=GetMouseWheelMove();
    if (penradius < 2) penradius = 2;
    if (penradius > 50) penradius = 50;
    
    // if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && tools == PEN)
    // {
    //   if (!penStarted)
    //   {
    //     prevMouse = curr;
    //     penStarted = true;
    //   }
      
    //   float dx = curr.x - prevMouse.x;
    //   float dy = curr.y - prevMouse.y;
    //   float dist = sqrt(dx*dx + dy*dy);

    //   float spacing = 0.1f;

    //   float t;
    //   float x;
    //   float y;

    //   for (float i = 0; i < dist; i += spacing){
        
    //     t = i / dist;
    //     x = prevMouse.x + dx * t;
    //     y = prevMouse.y + dy * t;
        
    //     // circles[(int)i].centerY = y;
    //     // circles[(int)i].radius = penradius;
    //     // circles[(int)i].color = RED;
        
    //     // DrawCircle(x, y, penradius, RED);
    //   }
      
    //   circleCount +=1;
    //   circles = realloc(circles,(sizeof(Circle) * circleCount));

    //   shapesCount += 1;
    //   shapes = realloc(shapes,sizeof(Shape) * shapesCount);
      
    //   shapes[shapesCount - 1].type = SHAPE_FREELINE;
    //   shapes[shapesCount - 1].shape.freeline = (FreeLine) {
    //     .id = shapesCount -1,
    //     .circles = malloc(sizeof(Circle) * 1),
    //     .circlesCount = circleCount
    //   };

    //   FreeLine *currentLine = &shapes[shapesCount -1].shape.freeline;

    //   currentLine->circles[circleCount -1] = (Circle) {
    //     .id = circleCount -1,
    //     .centerX = x,
    //     .centerY = y,
    //     .radius = penradius,
    //     .color = RED
    //   };

    //   // BeginTextureMode(target);
    //     // ClearBackground(BLACK);
    //     // for (float i = 0; i < dist; i += spacing){
    //     //   float t = i / dist;

    //     //   float x = prevMouse.x + dx * t;
    //     //   float y = prevMouse.y + dy * t;

    //     //   DrawCircle(x, y, penradius, RED);
    //     // }
        
    //     // DrawRectangle(10,10,100,100,BLACK);
    //     // DrawText(TextFormat("%d a",curr),10,10,20,RED);
    //     // DrawText(TextFormat("%d bv",prevMouse),20,30,20,RED);
    //   // EndTextureMode();
      
    //   prevMouse = curr;
    // }
    // else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && tools == PEN){
    //   penStarted = false;
    // }

    
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
      EndTextureMode();
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == LINE) {
      mouseIsDown = false;
      shapesCount+=1;
      shapes = realloc(shapes,sizeof(Shape) * shapesCount);
      
      shapes[shapesCount - 1].type = SHAPE_LINE;
      shapes[shapesCount - 1].shape.line = (Line) {
        .color = GREEN,
        .id = shapesCount -1,
        .startPosX = pm.x,
        .startPosY = pm.y,
        .endPosX = a.x,
        .endPosY = a.y,
      };

      BeginTextureMode(target2);
        ClearBackground(BLACK);
      EndTextureMode();
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
      EndTextureMode();
    }
    else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == RECTANGLE){
      mouseIsDown = false;
      shapesCount+=1;
      shapes = realloc(shapes,sizeof(Shape) * shapesCount);
      
      shapes[shapesCount - 1].type = SHAPE_RECTANGLE;
      shapes[shapesCount - 1].shape.rectangle = (Rectangled) {
        .color = BLUE,
        .x = pm.x,
        .y = pm.y,
        .width = a.x-pm.x,
        .height = a.y-pm.y,
        .id = shapesCount -1,
      }; 

      BeginTextureMode(target2);
        ClearBackground(BLACK);
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

      float dx = a.x-pm.x;
      float dy = a.y -pm.y;
      float radius = sqrt(dx*dx + dy*dy);

      BeginTextureMode(target2);
        ClearBackground(BLACK);
        DrawCircleLines((pm.x+a.x)/2,(pm.y+a.y)/2,radius/2,PINK);
      EndTextureMode();
    } else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == CIRCLE) {
      mouseIsDown = false;
      int x= GetMouseX();
      int y= GetMouseY();

      float dx = a.x-pm.x;
      float dy = a.y -pm.y;
      float radius = sqrt(dx*dx + dy*dy);

      shapesCount+=1;
      shapes = realloc(shapes,sizeof(Shape) * shapesCount);
      
      shapes[shapesCount - 1].type = SHAPE_CIRCLE;
      shapes[shapesCount - 1].shape.circle = (Circle) {
        .id = shapesCount -1,
        .centerX = (pm.x+a.x)/2,
        .centerY = (pm.y+a.y)/2,
        .radius = radius/2,
        .color = PINK
      };

      BeginTextureMode(target2);
        ClearBackground(BLACK);
      EndTextureMode();
      
    }
    
    if(IsKeyPressed(KEY_A)){
      free(shapes);
      shapes = NULL;
      shapesCount = 0;
    }

    BeginTextureMode(target);
    ClearBackground(BLACK);
    DrawTextureRec(target2.texture, (Rectangle){ 0, 0, (float)target2.texture.width, (float)-target2.texture.height }, (Vector2) { 0, 0 }, WHITE);

      for(int i = 0; i < shapesCount; i++){
        if(shapes[i].type == SHAPE_LINE) {
          DrawLine(
            shapes[i].shape.line.startPosX,
            shapes[i].shape.line.startPosY,
            shapes[i].shape.line.endPosX,
            shapes[i].shape.line.endPosY,
            shapes[i].shape.line.color
          );
        } else if(shapes[i].type == SHAPE_RECTANGLE) {
          DrawRectangleLines(
            shapes[i].shape.rectangle.x,
            shapes[i].shape.rectangle.y,
            shapes[i].shape.rectangle.width,
            shapes[i].shape.rectangle.height,
            shapes[i].shape.rectangle.color
          );
        } else if(shapes[i].type == SHAPE_CIRCLE) {
          DrawCircleLines(
            shapes[i].shape.circle.centerX,
            shapes[i].shape.circle.centerY,
            shapes[i].shape.circle.radius,
            shapes[i].shape.circle.color
          );
        } 
        // else if(shapes[i].type == SHAPE_FREELINE) {
        //   // int length = sizeof(shapes[i].shape.freeline.circles)/sizeof(Circle);
        //   FreeLine line = shapes[i].shape.freeline;
        //   for(int j = 0; j < line.circlesCount; j++){
        //     DrawCircle(
        //       shapes[i].shape.freeline.circles[j].centerX,
        //       shapes[i].shape.freeline.circles[j].centerY,
        //       shapes[i].shape.freeline.circles[j].radius,
        //       shapes[i].shape.freeline.circles[j].color
        //     );
        //   }
        // }
      }
    EndTextureMode();
    BeginDrawing();
    DrawTextureRec(target2.texture, (Rectangle){ 0, 0, (float)target2.texture.width, (float)-target2.texture.height }, (Vector2) { 0, 0 }, WHITE);
    DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
    EndDrawing();
  }
    
  CloseWindow();
    
  return 0;
}
