#include "gui.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "windows.h"
#include "utils.h"
#include "localfiles.h"
#include "Enigma2.h"


namespace Windows {
    void NetworkWindow(bool *focus, bool *first_item) {
        Windows::SetupWindow();
		static unsigned int item_current_idx = 0;
        std::vector<std::string> topmenu = {"Local Files","Network","Enigma2"};
		
        if (ImGui::Begin("Network Browser", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_MenuBar)) {
            if (!*focus) {
                ImGui::SetNextWindowFocus();
                *focus = true;
            }
			if(item.networkselect){
				if (ImGui::BeginListBox("Network Source Menu",ImVec2(1280.0f, 720.0f))){
					for(unsigned int n=0;n<item.networksources.size();n++){
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(item.networksources[n].name.c_str(), is_selected)){
									
							item.networkselect = false;
							item.networkurl = item.networksources[n].url;
							urlschema thisurl = Utility::parseUrl(item.networksources[n].url);
							item.networklastpath = thisurl.path;
							if(thisurl.scheme == "http" || thisurl.scheme == "https"){
								item_current_idx = 0;
								httpdir->setUrl(item.networkurl = item.networksources[n].url);
								item.networkentries = httpdir->dirList("");
								std::sort(item.networkentries.begin(),item.networkentries.end(),Utility::compare);
							}
							if(thisurl.scheme == "ftp"){
								netbuf *ftp_con = nullptr;
								printf("FTP CONNECT %s\n",thisurl.server.c_str());
								if(thisurl.port == "")thisurl.port = "21";
								std::string ftphost = thisurl.server+std::string(":")+thisurl.port;
								item_current_idx = 0;
								if (!FtpConnect(ftphost.c_str(), &ftp_con)) {
									printf("could not connect to ftp server\n");
								}else{
									
									if (!FtpLogin(thisurl.user.c_str(), thisurl.pass.c_str(), ftp_con)) {
									printf("could not connect to ftp server\n");
										FtpQuit(ftp_con);
									}else{
										item.networkentries = FtpDirList(thisurl.path.c_str(), ftp_con);
										std::sort(item.networkentries.begin(),item.networkentries.end(),Utility::compare);
									}
								}
							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
								
					}
				}
				ImGui::EndListBox();
			}
			else{
				if (ImGui::BeginMenuBar()) {
					ImGui::Text("current path: %s",item.networklastpath.c_str());
					ImGui::EndMenuBar();
				}
				if (ImGui::BeginListBox("Network Browser Menu",ImVec2(1280.0f, 720.0f))){
					//ImGui::Text("Current Dir: %s\n",item.networklastpath.c_str());
					int total_w = ImGui::GetContentRegionAvail().x;
					for(unsigned int n=0;n<item.networkentries.size();n++){
						if(item.networkentries[n].isDir || FS::GetFileType(item.networkentries[n].name) != FileTypeNone){
							if(item.networkentries[n].isDir){
								ImGui::Image((void*)(intptr_t)FolderTexture.id, ImVec2(40,40));
							}else{
								ImGui::Image((void*)(intptr_t)FileTexture.id, ImVec2(40,40));
							}
							ImGui::SameLine();
							ImGui::SetCursorPos({ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + (40 - ImGui::GetFont()->FontSize) / 2});
						
							urlschema thisurl = Utility::parseUrl(item.networkurl);
							const bool is_selected = (item_current_idx == n);
							if (ImGui::Selectable(item.networkentries[n].name.c_str(), is_selected)){
										
								if(item.networkentries[n].isDir){
									item_current_idx = 0;
									if(thisurl.scheme == "http" || thisurl.scheme == "https"){
										item.networkentries = httpdir->dirList(item.networkentries[n].path);
										item.networklastpath = httpdir->getCurrentRelPath();
										std::sort(item.networkentries.begin(),item.networkentries.end(),Utility::compare);
									}
											
									if(thisurl.scheme == "ftp"){
										netbuf *ftp_con = nullptr;
										if(thisurl.port == "")thisurl.port = "21";
										std::string ftphost = thisurl.server+std::string(":")+thisurl.port;
										if (!FtpConnect(ftphost.c_str(), &ftp_con)) {
											printf("could not connect to ftp server\n");
										}else{
											
											if (!FtpLogin(thisurl.user.c_str(), thisurl.pass.c_str(), ftp_con)) {
												printf("could not connect to ftp server\n");
												FtpQuit(ftp_con);
											}else{
												std::string dirpath = item.networkentries[n].path + item.networkentries[n].name;
												item.networklastpath = dirpath;
												item.networkentries = FtpDirList(dirpath.c_str(), ftp_con);
												std::sort(item.networkentries.begin(),item.networkentries.end(),Utility::compare);
											}
										}
									}
								}else{
											
									urlschema thisurl = Utility::parseUrl(item.networkurl);
									if(thisurl.scheme == "http" || thisurl.scheme == "https"){
										std::string openurl = httpdir->getUrl() + httpdir->getCurrentRelPath() + item.networkentries[n].path;
										const char *cmd[] = {"loadfile", openurl.c_str(), NULL};
										mpv_command_async(mpv->getHandle(), 0, cmd);
									}
									if(thisurl.scheme == "ftp"){
										std::string openurl = thisurl.scheme + std::string("://") + thisurl.user + std::string(":") + thisurl.pass + std::string("@") + thisurl.server + std::string("/") + item.networkentries[n].path + item.networkentries[n].name;
										const char *cmd[] = {"loadfile", openurl.c_str(), NULL};
										mpv_command_async(mpv->getHandle(), 0, cmd);
									}
								}
							}
							if (is_selected)
								ImGui::SetItemDefaultFocus();
							if(!item.networkentries[n].isDir && thisurl.scheme != "http"){
								ImGui::SameLine(total_w-150);
								ImGui::Text("%s",Utility::humanSize(item.networkentries[n].size).c_str());
							}
						}
								
					}
						
				}
				ImGui::EndListBox();
			}
		}
		Windows::ExitWindow();
	}
}