#pragma once

#include "resource.h"

void DrawALine(HDC hdc, COLORREF color, int width, int startX, int startY, int stopX, int stopY) {
	HPEN hLinePen;
	HPEN hPenOld;
	hLinePen = CreatePen(PS_SOLID, width, color);
	hPenOld = (HPEN)SelectObject(hdc, hLinePen);

	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, stopX, stopY);

	SelectObject(hdc, hPenOld);
	DeleteObject(hLinePen);
}

