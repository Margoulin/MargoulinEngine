#include "PolygonRenderResource.hpp"

#include "Engine.hpp"
#include "GraphicalLibrary.hpp"
#include "D3D11Context.hpp"
#include <string>

auto	PolygonRenderResource::AddPoint(Vector2F const& value) -> void
{
	points.push_back(value);
	isConstructionDirty = true;
}

auto	PolygonRenderResource::ConstructGeometry() -> void
{
	if (points.size() == 0)
		return;
	GraphicalLibrary* lib = Engine::GetInstance()->GetService<GraphicalLibrary>("Renderer");
	D3D11Context* context = (D3D11Context*)lib->GetContext();

	HRESULT hr = context->d2dFactory->CreatePathGeometry(&polygonGeometry);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink *pSink = NULL;

		hr = polygonGeometry->Open(&pSink);
		if (SUCCEEDED(hr))
		{
			pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

			pSink->BeginFigure(
				D2D1::Point2F(points[0].x, points[0].y),
				D2D1_FIGURE_BEGIN_FILLED);
			
			unsigned int size = points.size();
			D2D1_POINT_2F* tempPoints = new D2D1_POINT_2F[size];
			for (unsigned int pos = 1; pos < points.size(); pos++)
				tempPoints[pos - 1] = D2D1::Point2F(points[pos].x, points[pos].y);
			tempPoints[size - 1] = D2D1::Point2F(points[0].x, points[0].y);

			pSink->AddLines(tempPoints, size);
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		hr = pSink->Close();
		if (pSink)
			pSink->Release();
	}
	isConstructionDirty = false;
}

auto	PolygonRenderResource::ClearGeometry() -> void
{
	points.clear();
	polygonGeometry = nullptr;
}

#include <imgui.h>
#ifdef _DEBUG

auto	PolygonRenderResource::ImGuiUpdate() -> void
{
	ImGui::Value("Construction Dirty", &isConstructionDirty);
	ImGui::Value("Points number", (unsigned int)points.size());
	if (ImGui::TreeNode("Points"))
	{
		for (unsigned int pos = 0; pos < points.size(); pos++)
		{
			std::string	tempName = "Point " + std::to_string(pos);
			ImGui::DragFloat2(tempName.c_str(), (float*)&points[pos].x);
		}
		ImGui::TreePop();
	}
	ImGui::DragFloat2("New Point", (float*)&tempPointValue.x);
	if (ImGui::Button("Add point"))
		AddPoint(tempPointValue);

	ImGui::DragInt("IDX To Remove", (int*)&idxToRemove, 1.0f, 0, points.size() - 1);
	if (ImGui::Button("Remove point"))
		points.erase(points.begin() + idxToRemove);

	if (ImGui::Button("Construct Geometry"))
		ConstructGeometry();
	if (ImGui::Button("Clear polygon"))
		ClearGeometry();
}

#endif // _DEBUG
