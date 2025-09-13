#include "DebugUI.h"

#include <glm/gtc/type_ptr.hpp>
#include <magic_enum/magic_enum.hpp>

#include "Components/Grid.h"
#include "Components/Light.h"
#include "Components/Primitives.h"
#include "DebugData.h"
#include "Mouse.h"
#include "imgui.h"

namespace Mir {

    DebugUI::DebugUI(DebugData* debugData) : data(debugData) {}

    void DebugUI::render() {
        if (!data) {
            ImGui::Begin("Debug Information");
            ImGui::Text("No debug data available (nullptr).");
            ImGui::End();
            return;
        }

        ImGui::Begin("Debug Information");

        if (ImGui::TreeNode("Camera Info")) {
            renderCameraInfo();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("ScreenToWorld Debug")) {
            renderScreenToWorld();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Mouse Info")) {
            renderMouseInfo();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Timing Info")) {
            renderTimingInfo();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Miscellaneous")) {
            trashPile();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Objects")) {
            objects();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Lights")) {
            lights();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Drag & Drop Info")) {
            renderDragDropInfo();
            ImGui::TreePop();
        }
        if (data->grid) {
            if (ImGui::TreeNode("Grid Config")) {
                grid();
                ImGui::TreePop();
            }
        }

        ImGui::End();
    }

    void DebugUI::renderCameraInfo() {}

    void DebugUI::renderScreenToWorld() {
        ImGui::Text(
            "World Position: (%.2f, %.2f, %.2f)", data->worldPosition.x, data->worldPosition.y, data->worldPosition.z);

        ImGui::Separator();

        ImGui::Text("Inverse Projection Matrix:");
        for (int i = 0; i < 4; ++i) {
            ImGui::Text(
                "[%.3f, %.3f, %.3f, %.3f]",
                data->inverseProjection[i][0],
                data->inverseProjection[i][1],
                data->inverseProjection[i][2],
                data->inverseProjection[i][3]);
        }

        ImGui::Text("Inverse View Matrix:");
        for (int i = 0; i < 4; ++i) {
            ImGui::Text(
                "[%.3f, %.3f, %.3f, %.3f]",
                data->inverseView[i][0],
                data->inverseView[i][1],
                data->inverseView[i][2],
                data->inverseView[i][3]);
        }
    }

    void DebugUI::renderMouseInfo() {
        if (data->mouse) {
            ImGui::Separator();
            ImGui::Text("Mouse (X,Y) (%.1f, %.1f)", data->mouse->screen.x, data->mouse->screen.y);
            ImGui::Text("Viewport (X,Y) (%.1f, %.1f)", data->mouse->viewport.x, data->mouse->viewport.y);
            ImGui::Text("World (X,Y) (%.2f, %.2f)", data->mouse->world.x, data->mouse->world.y);

            ImGui::Separator();
            ImGui::Text("Mouse State (Raw): 0x%X", static_cast<unsigned int>(data->mouse->currentState));
            ImGui::Indent(10);
            ImGui::Text("Mouse State (Active):");

            for (const auto& state : magic_enum::enum_values<MouseState>()) {
                if (state == MouseState::NONE) {
                    continue;
                }
                bool isPressed = (data->mouse->currentState & state) == state;
                if (isPressed) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));  // Green for active
                    ImGui::Text("%s (ACTIVE)", magic_enum::enum_name(state).data());
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));  // Gray for inactive
                    ImGui::Text("%s (NOT ACTIVE)", magic_enum::enum_name(state).data());
                }
                ImGui::PopStyleColor();
            }
            ImGui::Unindent(10);
        }
    }

    void DebugUI::renderTimingInfo() {
        ImGui::Text("Delta Time: %.6f", data->deltaTime);
        ImGui::Text("Time: %.2f", data->time);
        ImGui::Text("FPS: %.1f", data->fps);
    }

    void DebugUI::trashPile() {
        if (ImGui::Checkbox("Wireframe", &data->showWireframe)) {
        }

        if (ImGui::Checkbox("MainWindow", &data->MainWindow)) {
            /* code */
        }
    }
    void DebugUI::objects() {
        for (auto& ob : *data->objects) {
            object(*ob);
        }
    }
    void DebugUI::object(Object& object) {
        if (object.isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255.0f, 0.5f, 255.0f, 1.0f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        }

        if (ImGui::TreeNode((void*)(intptr_t)object.id, "Object ID: %u", object.id)) {
            ImGui::PopStyleColor();
            ImGui::ColorEdit3("Color", (float*)&object.color);
            ImGui::DragFloat3("Position", glm::value_ptr(object.modelMatrix[3]), 0.1f);
            if (ImGui::BeginCombo("Draw Mode", magic_enum::enum_name(object.drawMode).data())) {
                for (auto drawMode : magic_enum::enum_values<DrawMode>()) {
                    bool isSelected = (object.drawMode == drawMode);
                    if (ImGui::Selectable(magic_enum::enum_name(drawMode).data(), isSelected)) {
                        object.drawMode = drawMode;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::Text("Vertices: %zu", object.vertices.size());
            ImGui::Text("Indices: %zu", object.indices.size());
            ImGui::Checkbox("Selected", &object.isSelected);

            ImGui::Separator();
            ImGui::Text("Dragged Object Vertices:");
            int vIdx = 0;
            for (const auto& v : object.vertices) {
                if (v.selected) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255.0f, 0.5f, 255.0f, 1.0f));
                } else {
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                }

                ImGui::Text("Vertex %d: (%.3f, %.3f, %.3f)", vIdx++, v.position.x, v.position.y, v.position.z);
                ImGui::PopStyleColor();
            }
            ImGui::TreePop();
        } else {
            ImGui::PopStyleColor();  // Pop outside if tree node is closed
        }
    }

    void DebugUI::lights() {
        if (!data->lights) return;
        int idx = 0;
        for (auto& light : *data->lights) {
            if (ImGui::TreeNode((void*)&light, "Light %d", idx++)) {
                ImGui::ColorEdit3("Color", glm::value_ptr(light.controls.color));
                ImGui::DragFloat("Intensity", &light.controls.intensity, 0.01f, 0.0f, 10.0f);
                glm::vec3 pos = glm::vec3(light.mesh.modelMatrix[3]);
                if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1f)) {
                    light.mesh.modelMatrix = glm::translate(glm::mat4(1.0f), pos);
                }
                ImGui::TreePop();
            }
        }
    }

    void DebugUI::renderDragDropInfo() {
        if (!data) return;
        // Assuming you have a pointer to Playground or DragDrop struct in DebugData
        const auto& dragDrop = data->dragDrop;  // Add DragDrop* dragDrop to DebugData

        ImGui::Text("Is Dragging Vertex: %s", dragDrop->isDraggingVertex ? "true" : "false");
        ImGui::Text("Object Index: %d", dragDrop->objectIdx);
        ImGui::Text("Vertex Index: %d", dragDrop->vertexIdx);
        ImGui::Text("Vertex global offset: %d", dragDrop->globalVboOffset);
        if (data->dragDrop && data->dragDrop->obj) {
            object(*data->dragDrop->obj);
            ImGui::Separator();
            ImGui::Text("Dragged Object Vertices:");
            int vIdx = 0;
            for (const auto& v : data->dragDrop->obj->vertices) {
                ImGui::Text("Vertex %d: (%.3f, %.3f, %.3f)", vIdx++, v.position.x, v.position.y, v.position.z);
            }
        }
    }
    void DebugUI::grid() {
        if (!data->grid) return;
        auto& config = data->grid->config;
        if (ImGui::DragFloat("Size", &config.size, 0.1f, 1.0f, 100.0f)) {
        }
        if (ImGui::ColorEdit3("Color", glm::value_ptr(config.color))) {
        }
        if (ImGui::DragFloat("Width", &config.lineWidth, 0.01f, 0.1f, 10.0f)) {
        }
        if (ImGui::DragFloat("Fade Distance", &config.fadeDistance, 0.01f, 0.0f, 10.0f)) {
        }
        if (ImGui::DragFloat("Fade Strength", &config.fadeStrength, 0.01f, 0.0f, 10.0f)) {
        }
        ImGui::Checkbox("Enable Fading", &config.enableFading);
    }
}  // namespace Mir