#include "gui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "windows.h"
#include "utils.h"
#include "localfiles.h"
#include "Enigma2.h"


namespace Windows {
    void FileBrowserWindow(bool *focus, bool *first_item) {
        Windows::SetupWindow();
		static unsigned int item_current_idx = 0;
        std::vector<std::string> topmenu = {"Local Files","Network","Enigma2"};
		
        if (ImGui::Begin("File Browser", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar)) {
            
            ImGui::SetNextWindowFocus();
            
			if (ImGui::BeginMenuBar()) {
				ImGui::Text("current path: %s",item.localpath.c_str());
				ImGui::EndMenuBar();
			}
			if (ImGui::BeginListBox("File Browser Menu",ImVec2(1280.0f, 720.0f))){
				int total_w = ImGui::GetContentRegionAvail().x;
				for (unsigned int n = 0; n < item.localfileentries.size(); n++){
					static int selected = -1;
					if(item.localfileentries[n].type == FsDirEntryType_Dir || FS::GetFileType(item.localfileentries[n].name) != FileTypeNone){
						if(item.localfileentries[n].type == FsDirEntryType_Dir){
							ImGui::Image((void*)(intptr_t)FolderTexture.id, ImVec2(40,40));
						}else{
							ImGui::Image((void*)(intptr_t)FileTexture.id, ImVec2(40,40));
						}
						ImGui::SameLine();
						ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + (40 - ImGui::GetFont()->FontSize) / 2});
						if (ImGui::Selectable(item.localfileentries[n].name, selected == n)){
							if(item.localfileentries[n].type == FsDirEntryType_Dir){
								item.first_item = true;
								item_current_idx = 0;
								if(item.localpath!="/"){
									item.localpath.append("/");
								}
								item.localpath.append(item.localfileentries[n].name);
								FS::GetDirList(item.localpath.c_str(),item.localfileentries);
							}
							else{
								std::string playfilepath = item.localpath;
								playfilepath.append("/");
								playfilepath.append(item.localfileentries[n].name);
								const char *cmd[] = {"loadfile", playfilepath.c_str(), NULL};
								mpv_command_async(libmpv->getHandle(), 0, cmd);
							}
						}
						if(item.localfileentries[n].type != FsDirEntryType_Dir){
							ImGui::SameLine(total_w-150);
							ImGui::Text("%s",Utility::humanSize(item.localfileentries[n].file_size).c_str());
						}
					}
					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				if (*first_item) {
					ImGui::SetFocusID(ImGui::GetID(item.localfileentries[0].name), ImGui::GetCurrentWindow());
					*first_item = false;
				}
			}	
			ImGui::EndListBox();		
        }
        Windows::ExitWindow();
    }
}