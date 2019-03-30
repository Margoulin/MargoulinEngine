#include "Renderer2DComponent.hpp"

#include "Engine.hpp"
#include "GraphicalLibrary.hpp"
#include "RendererPipeline.hpp"

#ifndef VITA
#include "PolygonRenderResource.hpp"
#endif

auto	Renderer2DComponent::Draw() -> void
{
	auto* lib = Engine::GetInstance()->GetService<GraphicalLibrary>("Renderer");
	switch (currentType)
	{
		case RECTANGLE:
		{
			if (filled && !roundEdges)
				lib->GetRenderPipeline()->DrawFilledRectangle(rectanglePosition, rectangleSize, color);
			else if (!filled && !roundEdges)
				lib->GetRenderPipeline()->DrawRectangle(rectanglePosition, rectangleSize, color);
			else if (roundEdges && filled)
				lib->GetRenderPipeline()->DrawFilledRoundedRectangle(rectanglePosition, rectangleSize, edgedRadius, color);
			else if (roundEdges && !filled)
				lib->GetRenderPipeline()->DrawRoundedRectangle(rectanglePosition, rectangleSize, edgedRadius, color);
			break;
		}
		case CIRCLE:
		{
			if (filled)
				lib->GetRenderPipeline()->DrawFilledEllipse(circlePosition, circleRadius, color);
			else
				lib->GetRenderPipeline()->DrawEllipse(circlePosition, circleRadius, color);
			break;
		}
		case	LINE:
		{
			lib->GetRenderPipeline()->DrawLine(linePoint, linePoint2, color);
			break;
		}
		case POLYGON:
		{
#ifndef VITA
			if (filled)
				lib->GetRenderPipeline()->DrawFilledGeometry(&polygon, color);
			else
				lib->GetRenderPipeline()->DrawGeometry(&polygon, color);
#endif
			break;
		}
		default:
			break;
	}
}

#include <imgui/imgui.h>
#ifdef _DEBUG

auto	Renderer2DComponent::ImGuiUpdate() -> void
{
	Component::ImGuiUpdate();

	ImGui::Combo("2D Type", (int*)&currentType, "None\0Rectangle\0Circle\0Line\0Polygon\0");
	ImGui::ColorEdit4("Color", (float*)&color.x);
	ImGui::Checkbox("Filled", &filled);
	if (currentType == Renderer2DType::RECTANGLE)
	{
		ImGui::DragFloat2("Position", (float*)&rectanglePosition.x);
		ImGui::DragFloat2("Size", (float*)&rectangleSize.x);
		ImGui::Checkbox("Rounded", &roundEdges);
		if (roundEdges)
			ImGui::DragFloat2("Edges Radius", (float*)&edgedRadius.x);
	}
	else if (currentType == Renderer2DType::CIRCLE)
	{
		ImGui::DragFloat2("Position", (float*)&circlePosition.x);
		ImGui::DragFloat2("Radius", (float*)&circleRadius.x);
	}
	else if (currentType == Renderer2DType::LINE)
	{
		ImGui::DragFloat2("First Point", (float*)&linePoint);
		ImGui::DragFloat2("Second Point", (float*)&linePoint2);
	}
#ifndef VITA
	else if (currentType == Renderer2DType::POLYGON)
	{
		polygon.ImGuiUpdate();
	}
	if (currentType != Renderer2DType::POLYGON)
		polygon.ClearGeometry();
#endif
}

#endif