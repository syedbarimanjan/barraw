#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

#include <math.h>

typedef struct {
  int id;
  float x;
  float y;
  float width;
  float height;
  Color color;
  bool isSelected;
} Rectangled;

typedef struct {
  int id;
  int centerX;
  int centerY;
  float radius;
  Color color;
  bool isSelected;
} Circle;


typedef struct {
  int id;
  int startPosX;
  int startPosY;
  int endPosX;
  int endPosY;
  Color color;
  bool isSelected;
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
  SELECTION,
} Tools;

Tools tools;

int main() {
  InitWindow(2000, 1000, "Barraw");
  bool mouseWasPressed = false;
  bool mouseIsDown = false;

  int shapesCount = 0;
  Shape *shapes;
  shapes = malloc(sizeof(Shape) * shapesCount);

  int copiedShapesCount = 0;
  Shape *copiedShapes;
  copiedShapes = malloc(sizeof(Shape) * copiedShapesCount);

  int circleCount = 0;
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
    } else if(IsKeyPressed(KEY_V)){
      tools = SELECTION;
    }

    Vector2 curr = GetMousePosition();
    float mousewheel = GetMouseWheelMove();
    penradius+=GetMouseWheelMove();
    if (penradius < 2) penradius = 2;
    if (penradius > 50) penradius = 50;


    //todo: this is too slow adding so many elements and then moveing them is not good should change the datastruct.
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && tools == PEN) {
      if (!penStarted) {
        prevMouse = curr;
        penStarted = true;
      }

      float dx = curr.x - prevMouse.x;
      float dy = curr.y - prevMouse.y;
      float dist = sqrt(dx*dx + dy*dy);

      float spacing = 1.0f;

      for (float i = 0; i < dist; i += spacing) {
        float t = i / dist;
        float x = prevMouse.x + dx * t;
        float y = prevMouse.y + dy * t;
        BeginTextureMode(target2);
          DrawCircle(x, y, penradius, RED);
        EndTextureMode();

        circleCount += 1;
        circles = realloc(circles,sizeof(Circle) * circleCount);
        circles[circleCount -1] = (Circle) {
          .id = circleCount -1,
          .centerX = x,
          .centerY = y,
          .radius = penradius,
          .color = RED
        };
      }

      prevMouse = curr;
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == PEN){
      penStarted = false;
      shapesCount += 1;
      shapes = realloc(shapes,sizeof(Shape) * shapesCount);

      Circle *circlesCopy = malloc(sizeof(Circle) * circleCount);
      for(int i = 0; i < circleCount; i++){
        circlesCopy[i] = circles[i];
      }

      shapes[shapesCount - 1].type = SHAPE_FREELINE;
      shapes[shapesCount - 1].shape.freeline = (FreeLine) {
        .id = shapesCount -1,
        .circles = circlesCopy,
        .circlesCount = circleCount
      };
      circleCount = 0;
      free(circles);
      circles = NULL;
      BeginTextureMode(target2);
        ClearBackground(BLACK);
      EndTextureMode();
    }

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
    }
    else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == LINE) {
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
        .isSelected = true,
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
        .isSelected = true,
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
    }
    else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && tools == CIRCLE) {
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
        .color = PINK,
        .isSelected = true,
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

          Vector2 mouseDelta = GetMouseDelta();
          Vector2 currentMousePosition = GetMousePosition();

          Vector2 p1 = (Vector2) {
            .x = shapes[i].shape.line.startPosX,
            .y = shapes[i].shape.line.startPosY,
          };

          Vector2 p2 = (Vector2) {
            .x = shapes[i].shape.line.endPosX,
            .y = shapes[i].shape.line.endPosY,
          };

          if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointLine(currentMousePosition,p1,p2,10)){
            shapes[i].shape.line.isSelected = false;
          }


          if(tools == SELECTION && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 mouseDelta = GetMouseDelta();
            Vector2 currentMousePosition = GetMousePosition();

            Vector2 p1 = (Vector2) {
              .x = shapes[i].shape.line.startPosX,
              .y = shapes[i].shape.line.startPosY,
            };

            Vector2 p2 = (Vector2) {
              .x = shapes[i].shape.line.endPosX,
              .y = shapes[i].shape.line.endPosY,
            };

            if(CheckCollisionPointLine(currentMousePosition,p1,p2,5)){
              shapes[i].shape.line.isSelected = true;
              DrawFPS(10,10);
              DrawLineV(p1,p2,RED);
              DrawCircle(p1.x,p1.y,10,RED);
              shapes[i].shape.line.startPosX += mouseDelta.x;
              shapes[i].shape.line.startPosY += mouseDelta.y;

              DrawCircle(p2.x,p2.y,10,RED);
              shapes[i].shape.line.endPosX += mouseDelta.x;
              shapes[i].shape.line.endPosY += mouseDelta.y;
            }

            if(CheckCollisionPointLine(currentMousePosition,p1,p2,10) && IsKeyDown(KEY_DELETE)){
              shapes[i].shape.line = (Line){};
            }
          }
          // todo: diagonol lines bounding box is squinted.
          if(shapes[i].shape.line.isSelected){
            Vector2 point1UpLine = (Vector2) {
              .x = shapes[i].shape.line.startPosX - 10,
              .y = shapes[i].shape.line.startPosY - 10,
            };

            Vector2 p2UpLine = (Vector2) {
              .x = shapes[i].shape.line.endPosX - 10,
              .y = shapes[i].shape.line.endPosY - 10,
            };

            Vector2 point1DownLine = (Vector2) {
              .x = shapes[i].shape.line.startPosX + 10,
              .y = shapes[i].shape.line.startPosY + 10,
            };

            Vector2 point2DownLine = (Vector2) {
              .x = shapes[i].shape.line.endPosX + 10,
              .y = shapes[i].shape.line.endPosY + 10,
            };

            DrawLineDashed(point1UpLine,p2UpLine,10,10,WHITE);
            DrawLineDashed(point1DownLine,point2DownLine,10,10,WHITE);
            DrawLineDashed(point1UpLine,point1DownLine,10,10,WHITE);
            DrawLineDashed(p2UpLine,point2DownLine,10,10,WHITE);
          }

          if(shapes[i].shape.line.isSelected && IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)){
            copiedShapesCount+=1;
            copiedShapes = realloc(copiedShapes,sizeof(Shape) * copiedShapesCount);

            Line modLine = (Line){
              .startPosX = shapes[i].shape.line.startPosX + 10,
              .startPosY = shapes[i].shape.line.startPosY + 10,
              .endPosX = shapes[i].shape.line.endPosX + 10,
              .endPosY = shapes[i].shape.line.endPosY + 10,
              .color = shapes[i].shape.line.color,
              .isSelected = true,
            };
            copiedShapes[copiedShapesCount - 1].type = SHAPE_LINE;
            copiedShapes[copiedShapesCount - 1].shape.line = modLine;
            tools = SELECTION;
          }

          if(CheckCollisionCircles(p1,10,currentMousePosition,10) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            shapes[i].shape.line.isSelected = false;
            DrawFPS(10,100);
            DrawCircleV(p1,10,RED);
            shapes[i].shape.line.startPosX += mouseDelta.x;
            shapes[i].shape.line.startPosY += mouseDelta.y;
          }
          else if (CheckCollisionCircles(p2,10,currentMousePosition,10) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
              shapes[i].shape.line.isSelected = false;
              DrawFPS(10,100);
              DrawCircleV(p2,10,RED);
              shapes[i].shape.line.endPosX += mouseDelta.x;
              shapes[i].shape.line.endPosY += mouseDelta.y;
          }

        } else if(shapes[i].type == SHAPE_RECTANGLE) {
          DrawRectangleLines(
            shapes[i].shape.rectangle.x,
            shapes[i].shape.rectangle.y,
            shapes[i].shape.rectangle.width,
            shapes[i].shape.rectangle.height,
            shapes[i].shape.rectangle.color
          );

          if(tools == SELECTION && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 mouseDelta = GetMouseDelta();
            Vector2 currentMousePosition = GetMousePosition();

            Rectangle rec = (Rectangle) {
              .x =shapes[i].shape.rectangle.x,
              .y =shapes[i].shape.rectangle.y,
              .width =shapes[i].shape.rectangle.width,
              .height =shapes[i].shape.rectangle.height,
            };
            if(CheckCollisionPointRec(currentMousePosition,rec)){
              shapes[i].shape.rectangle.isSelected = true;
              DrawFPS(10,10);
              DrawRectangleLinesEx(rec,2.0,RED);
              shapes[i].shape.rectangle.x += mouseDelta.x;
              shapes[i].shape.rectangle.y += mouseDelta.y;
            }

            if(CheckCollisionPointRec(currentMousePosition,rec) && IsKeyDown(KEY_DELETE)){
              shapes[i].shape.rectangle = (Rectangled) {};
            }
          }

          Vector2 currentMousePosition = GetMousePosition();
          Rectangle rec = (Rectangle) {
            .x =shapes[i].shape.rectangle.x - 10,
            .y =shapes[i].shape.rectangle.y - 10,
            .width =shapes[i].shape.rectangle.width + 20,
            .height =shapes[i].shape.rectangle.height + 20,
          };

          if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointRec(currentMousePosition,rec)){
            shapes[i].shape.rectangle.isSelected = false;
          }

          if(shapes[i].shape.rectangle.isSelected){
            // (left, top)    =============== (right, top)
            //       |                             |
            // (left, bottom) =============== (right, bottom)
            float left = fminf(shapes[i].shape.rectangle.x,shapes[i].shape.rectangle.x + shapes[i].shape.rectangle.width);
            float right = fmaxf(shapes[i].shape.rectangle.x,shapes[i].shape.rectangle.x + shapes[i].shape.rectangle.width);

            float top = fminf(shapes[i].shape.rectangle.y,shapes[i].shape.rectangle.y + shapes[i].shape.rectangle.height);
            float bottom = fmaxf(shapes[i].shape.rectangle.y,shapes[i].shape.rectangle.y + shapes[i].shape.rectangle.height);

            Vector2 point1UpLine = (Vector2) {
              .x = left - 10,
              .y = top - 10,
            };
            Vector2 point2UpLine = (Vector2) {
              .x = right + 10,
              .y = top - 10,
            };
            Vector2 point1DownLine = (Vector2) {
              .x = left - 10,
              .y = bottom + 10,
            };
            Vector2 point2DownLine = (Vector2) {
              .x = right + 10,
              .y = bottom + 10,
            };

            Vector2 center = (Vector2) {
              .x = (left + right) / 2,
              .y = (top + bottom) / 2
            };
            DrawCircle(center.x,center.y,5,WHITE);

            DrawLineDashed(point1UpLine,point2UpLine,10,10,WHITE);
            DrawLineDashed(point1DownLine,point2DownLine,10,10,WHITE);
            DrawLineDashed(point2DownLine,point2UpLine,10,10,WHITE);
            DrawLineDashed(point1DownLine,point1UpLine,10,10,WHITE);
          }

          if(shapes[i].shape.rectangle.isSelected && IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)){
            copiedShapesCount+=1;
            copiedShapes = realloc(copiedShapes,sizeof(Shape) * copiedShapesCount);

            Rectangled modifiedRectangle = (Rectangled){
              .x = shapes[i].shape.rectangle.x + 10,
              .y = shapes[i].shape.rectangle.y + 10,
              .width = shapes[i].shape.rectangle.width,
              .height = shapes[i].shape.rectangle.height,
              .color = shapes[i].shape.rectangle.color,
              .isSelected = true
            };
            copiedShapes[copiedShapesCount - 1].type = SHAPE_RECTANGLE;
            copiedShapes[copiedShapesCount - 1].shape.rectangle = modifiedRectangle;
            tools = SELECTION;
          }

          float left = fminf(shapes[i].shape.rectangle.x,shapes[i].shape.rectangle.x + shapes[i].shape.rectangle.width);
          float right = fmaxf(shapes[i].shape.rectangle.x,shapes[i].shape.rectangle.x + shapes[i].shape.rectangle.width);

          float top = fminf(shapes[i].shape.rectangle.y,shapes[i].shape.rectangle.y + shapes[i].shape.rectangle.height);
          float bottom = fmaxf(shapes[i].shape.rectangle.y,shapes[i].shape.rectangle.y + shapes[i].shape.rectangle.height);

          Vector2 point1UpLine = (Vector2) {
            .x = left - 10,
            .y = top - 10,
          };
          Vector2 point2UpLine = (Vector2) {
            .x = right + 10,
            .y = top - 10,
          };
          Vector2 point1DownLine = (Vector2) {
            .x = left - 10,
            .y = bottom + 10,
          };
          Vector2 point2DownLine = (Vector2) {
            .x = right + 10,
            .y = bottom + 10,
          };

          Vector2 mouseDelta = GetMouseDelta();
          if(shapes[i].shape.rectangle.isSelected && CheckCollisionPointLine(currentMousePosition,point1UpLine,point2UpLine,10) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            shapes[i].shape.rectangle.isSelected = true;
            DrawLineDashed(point1UpLine,point2UpLine,10,10,RED);
            shapes[i].shape.rectangle.y += mouseDelta.y;
            shapes[i].shape.rectangle.height -= mouseDelta.y;
          }
          else if(shapes[i].shape.rectangle.isSelected && CheckCollisionPointLine(currentMousePosition,point1DownLine,point2DownLine,10) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            shapes[i].shape.rectangle.isSelected = true;
            DrawLineDashed(point1DownLine,point2DownLine,10,10,RED);
            shapes[i].shape.rectangle.height += mouseDelta.y;
          }
          else if(shapes[i].shape.rectangle.isSelected && CheckCollisionPointLine(currentMousePosition,point2DownLine,point2UpLine,10) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            shapes[i].shape.rectangle.isSelected = true;
            DrawLineDashed(point2DownLine,point2UpLine,10,10,RED);
            shapes[i].shape.rectangle.width += mouseDelta.x;
          }
          else if(shapes[i].shape.rectangle.isSelected && CheckCollisionPointLine(currentMousePosition,point1DownLine,point1UpLine,10) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            shapes[i].shape.rectangle.isSelected = true;
            DrawLineDashed(point1DownLine,point1UpLine,10,10,RED);
            shapes[i].shape.rectangle.x += mouseDelta.x;
            shapes[i].shape.rectangle.width -= mouseDelta.x;
          }

        } else if(shapes[i].type == SHAPE_CIRCLE) {
          DrawCircleLines(
            shapes[i].shape.circle.centerX,
            shapes[i].shape.circle.centerY,
            shapes[i].shape.circle.radius,
            shapes[i].shape.circle.color
          );

          if(tools == SELECTION && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 mouseDelta = GetMouseDelta();
            Vector2 currentMousePosition = GetMousePosition();

            Vector2 circleCenter = (Vector2) {
              .x = shapes[i].shape.circle.centerX,
              .y = shapes[i].shape.circle.centerY,
            };

            float circleRadius = shapes[i].shape.circle.radius;

            if(CheckCollisionPointCircle(currentMousePosition,circleCenter,circleRadius)){
              shapes[i].shape.circle.isSelected = true;
              DrawFPS(10,10);
              DrawCircleLinesV(circleCenter,circleRadius,RED);
              shapes[i].shape.circle.centerX += mouseDelta.x;
              shapes[i].shape.circle.centerY += mouseDelta.y;
            }
            if(CheckCollisionPointCircle(currentMousePosition,circleCenter,circleRadius) && IsKeyDown(KEY_DELETE)){
              shapes[i].shape.circle = (Circle){};
            }
          }

          Vector2 currentMousePosition = GetMousePosition();
          Vector2 circleCenter = (Vector2) {
            .x = shapes[i].shape.circle.centerX,
            .y = shapes[i].shape.circle.centerY,
          };
          float circleRadius = shapes[i].shape.circle.radius;

          if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !CheckCollisionPointCircle(currentMousePosition,circleCenter,circleRadius)){
            shapes[i].shape.circle.isSelected = false;
          };

          if(shapes[i].shape.circle.isSelected){
            Vector2 circleTopLeft = (Vector2) {
              .x = shapes[i].shape.circle.centerX - shapes[i].shape.circle.radius - 10,
              .y = shapes[i].shape.circle.centerY - shapes[i].shape.circle.radius - 10,
            };

            Vector2 circleTopRight = (Vector2) {
              .x = shapes[i].shape.circle.centerX + shapes[i].shape.circle.radius + 10,
              .y = shapes[i].shape.circle.centerY - shapes[i].shape.circle.radius - 10,
            };

            Vector2 circleBottomRight = (Vector2) {
              .x = shapes[i].shape.circle.centerX + shapes[i].shape.circle.radius + 10,
              .y = shapes[i].shape.circle.centerY + shapes[i].shape.circle.radius + 10,
            };

            Vector2 circleBottomLeft = (Vector2) {
              .x = shapes[i].shape.circle.centerX - shapes[i].shape.circle.radius - 10,
              .y = shapes[i].shape.circle.centerY + shapes[i].shape.circle.radius + 10,
            };

            DrawLineDashed(circleTopLeft, circleTopRight, 10, 10, WHITE);
            DrawLineDashed(circleBottomLeft, circleBottomRight, 10, 10, WHITE);
            DrawLineDashed(circleTopLeft, circleBottomLeft, 10, 10, WHITE);
            DrawLineDashed(circleTopRight, circleBottomRight, 10, 10, WHITE);
          }

          if(shapes[i].shape.circle.isSelected && IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)){
            copiedShapesCount+=1;
            copiedShapes = realloc(copiedShapes,sizeof(Shape) * copiedShapesCount);

            Circle modifiedCircle = (Circle){
              .centerX = shapes[i].shape.circle.centerX + 10,
              .centerY = shapes[i].shape.circle.centerY + 10,
              .radius = shapes[i].shape.circle.radius,
              .color = shapes[i].shape.circle.color,
              .isSelected = true
            };
            copiedShapes[copiedShapesCount - 1].type = SHAPE_CIRCLE;
            copiedShapes[copiedShapesCount - 1].shape.circle = modifiedCircle;
            tools = SELECTION;
          }

          // Vector2 mouseDelta = GetMouseDelta();
          // if(shapes[i].shape.rectangle.isSelected && CheckCollisionCircles(circleCenter,circleRadius,currentMousePosition,10) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
          //   shapes[i].shape.rectangle.isSelected = true;
          //   DrawFPS(10, 100);
          //   shapes[i].shape.circle.centerX += mouseDelta.x;
          //   // shapes[i].shape.rectangle.height -= mouseDelta.y;
          // }

        } else if(shapes[i].type == SHAPE_FREELINE) {
          FreeLine line = shapes[i].shape.freeline;
          for(int j = 0; j < line.circlesCount; j++){
            DrawCircle(
              shapes[i].shape.freeline.circles[j].centerX,
              shapes[i].shape.freeline.circles[j].centerY,
              shapes[i].shape.freeline.circles[j].radius,
              shapes[i].shape.freeline.circles[j].color
            );

            // todo: combine the circle into a single line and then make that line moveable.
            // if(tools == SELECTION && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            //   Vector2 mouseDelta = GetMouseDelta();
            //   Vector2 currentMousePosition = GetMousePosition();
            //   Vector2 circleCenter = (Vector2) {
            //     .x = shapes[i].shape.freeline.circles[j].centerX,
            //     .y = shapes[i].shape.freeline.circles[j].centerY,
            //   };
            //   float circleRadius = shapes[i].shape.freeline.circles[j].radius;
            //   if(CheckCollisionPointCircle(currentMousePosition,circleCenter,circleRadius)){
            //     DrawFPS(10,10);
            //     DrawCircleLinesV(circleCenter,circleRadius,WHITE);
            //     shapes[i].shape.freeline.circles[j].centerX += mouseDelta.x;
            //     shapes[i].shape.freeline.circles[j].centerY += mouseDelta.y;
            //   }
            // }
          }

        }


      }

      if(copiedShapesCount > 0){
        int oldShapesCount = shapesCount;
        shapesCount += copiedShapesCount;
        shapes = realloc(shapes,sizeof(Shape) * shapesCount);

        for(int i = 0; i < copiedShapesCount; i++){
          shapes[oldShapesCount + i] = copiedShapes[i];
        }

        copiedShapesCount = 0;
        free(copiedShapes);
        copiedShapes = NULL;
      }
    EndTextureMode();

    BeginDrawing();
    DrawTextureRec(target2.texture, (Rectangle){ 0, 0, (float)target2.texture.width, (float)-target2.texture.height }, (Vector2) { 0, 0 }, WHITE);
    DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);
    EndDrawing();
  }

  UnloadRenderTexture(target);
  UnloadRenderTexture(target2);
  CloseWindow();

  return 0;
}
