#include "render.hpp"
#include "object.h"

#include <math.h>

void DrawTextByCenter(Texture tex, Vector2 center, float rotation, float scale, Color tint, bool flipY) {
	int flip = (flipY) ? -1 : 1;
	Rectangle src = {0, 0, tex.width, tex.height * flip};
	Rectangle dest = {center.x, center.y, scale * tex.width, scale * tex.height};
	Vector2 origin = {scale * tex.width / 2.0f, scale * tex.height / 2.0f};

	DrawTexturePro(tex, src, dest, origin, rotation, tint);
}

void UpdateTextureFromImage(Texture2D* texture, Image* img, int width, int height) {
	bool changed = (img->width != width || img->height != height);
	if (changed) {
		ImageResizeNN(img, width, height);
		UnloadTexture(*texture);
		*texture = LoadTextureFromImage(*img);
	}
}

void UpdateRender(RenderTexture* rt, RenderList list, int screenWidth, int screenHeight) {
//	Vector2 screenSize = {(float)screenWidth, (float)screenHeight};
//	Maze* mazeInfo = rota->GetMazeInfo(rota);
//	Vector2 mazeSize = {(float)mazeInfo->col, (float)mazeInfo->row};
//	double cell = MeasureCellSize(screenSize, mazeSize);
//	
//	float newW = cell 
//	bool changed = (rt->texture.width != screenWidth || rt->texture.height != screenHeight);
//	if (changed) {
//		UnloadRenderTexture(*rt);
//		*rt = LoadRenderTexture(screenWidth, screenHeight);
//		rota->base.Resize((Obj*)rota, screenWidth, screenHeight);
//		Maze* mazeInfo = ((RotaObj*)rota)->GetMazeInfo(rota);
//		double cell = MeasureCellSize((Vector2){screenWidth, screenHeight}, (Vector2){mazeInfo->col, mazeInfo->row});
//		mobi->base.Resize((Obj*)mobi, cell, cell);
//	}

	MobiObj** mobiLs = list.mobiList;
	RotaObj** rotaLs = list.rotaList;
	size_t mobiCnt = list.mobiCount;
	size_t rotaCnt = list.rotaCount;

	for (size_t cnt = 0; cnt < rotaCnt; cnt++) {
		((Obj*)rotaLs[cnt])->Resize((Obj*)rotaLs[cnt], screenWidth, screenHeight);
	}
	Vector2 cellSize = ((Obj*)rotaLs[0])->GetSize((Obj*)rotaLs[0]);
	for (size_t cnt = 0; cnt < mobiCnt; cnt++) {
		((Obj*)mobiLs[cnt])->Resize((Obj*)mobiLs[cnt], (int)cellSize.x, (int)cellSize.y);
	}
	Maze* mazeInfo = rotaLs[0]->GetMazeInfo(rotaLs[0]);
//	float rtW = cellSize.x * mazeInfo->col + 5;
//	float rtH = cellSize.y * mazeInfo->row + 5;
	float rtW = cellSize.x * mazeInfo->col;
	float rtH = cellSize.y * mazeInfo->row;
	if (rtW != rt->texture.width || rtH != rt->texture.height) {
		UnloadRenderTexture(*rt);
		*rt = LoadRenderTexture(rtW, rtH);
	}
}

void Render(RenderList list, RenderTexture* lastTexture, Vector2 alterV) {
	BeginTextureMode(*lastTexture);
//	BeginDrawing();
        ClearBackground(BLACK);

		for (size_t i = 0; i < list.rotaCount; i++) {
			list.rotaList[i]->base.Draw(&(list.rotaList[i]->base));
		}

		for (size_t i = 0; i < list.mobiCount; i++) {
			list.mobiList[i]->base.Draw(&(list.mobiList[i]->base));
		}
//	EndDrawing();
	EndTextureMode();

    // Sau đó xoay texture lên màn hình
//	float curAngle = (list.rotaList[0]->base.GetPos(&(list.rotaList[0]->base))).x;
//	
//	int width = lastTexture->texture.width;
//	int height = lastTexture->texture.height;

//    BeginDrawing();
//        ClearBackground(BLACK);
//
//        Rectangle source = { 0, 0, width, -height }; // cần - chiều cao
//        Rectangle dest = { alterV.x + width / 2, alterV.y + height / 2, width, height };
//        Vector2 origin = { width / 2, height / 2 };
//
//        DrawTexturePro(lastTexture->texture, source, dest, origin, curAngle, WHITE);
//    EndDrawing();

	BeginDrawing();
		ClearBackground(BLACK);
		DrawTextByCenter(lastTexture->texture, (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f}, 0.0f, 1.0f, WHITE, 1);
	EndDrawing();

//	FILE* log = fopen("rotate.log", "a");
//	fprintf(log, "rota == %.2f\t mobi == %.2f\n", list.rotaList[0]->base.GetAngleDegree(&(list.rotaList[0]->base)), list.mobiList[0]->base.GetAngleDegree(&(list.mobiList[0]->base)));
//	fclose(log);
}

double MeasureCellSize(Vector2 screenSize, Vector2 mazeSize) {
	int screenWidth = (int)screenSize.x, screenHeight = (int)screenSize.y;
	int col = (int)mazeSize.x, row = (int)mazeSize.y;

	// the code below is only for centering the maze on screen
	int minSize = (screenWidth < screenHeight) ? screenWidth : screenHeight;
	return minSize / sqrt(col * col + row * row);
}

Vector2 MeasureAlterVec(Vector2 screenSize, Vector2 mazeSize, double cellSize) {
	float screenWidth = screenSize.x, screenHeight = screenSize.y;
	int col = (int)mazeSize.x, row = (int)mazeSize.y;

	// the code below is only for centering the maze on screen
	float halfMazeWidth = cellSize * col / 2.0f;
	float halfMazeHeight = cellSize * row / 2.0f;

	return (Vector2) {screenWidth / 2.0f - halfMazeWidth, screenHeight / 2.0f - halfMazeHeight};
}
