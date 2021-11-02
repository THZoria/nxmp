#ifndef NXMP_LIBMPVMPV_H
#define NXMP_LIBMPVMPV_H

#include <string>
#include "fileInfo.h"
#include "config.h"

#include <mpv/client.h>
#include <mpv/render_gl.h>

struct decoderlist_struct{
	std::string codecname;
	std::string codecdesc;
};

extern Config *configini;

class libMpv{
public:

	libMpv(const std::string &configDir);
	
	~libMpv();
	
	void Pause();

	void Resume();

	void Stop();
	
	void seek(double position,bool osd);
	
	bool Stopped();
	
	bool Paused();
	
	int64_t getPosition();
	
	int64_t getVideoWidth();
	int64_t getVideoHeight();
	
	fileInfo *getFileInfo();
	
	void setVid(int id, bool osd);
	void setAid(int id, bool osd);
	void setSid(int id, bool osd);
	
	void setAspectRatio(double ratio,bool osd);
	
	void setBrightness(int value, bool osd);
	void setContrast(int value, bool osd);
	void setSaturation(int value, bool osd);
	void setGamma(int value, bool osd);
	void setHue(int value, bool osd);
	
	void setRotate(int value, bool osd);
	
	void setVolume(int value,bool osd);
	void setAudioDelay(double value,bool osd);
	

	
	void getfileInfo();
	
	
	mpv_handle *getHandle();

	mpv_render_context *getContext();
	std::vector<decoderlist_struct> getDecoderList();
	
	std::string mpv_version;
	std::string ffmpeg_version;
	
private:

    mpv_handle *handle = nullptr;
    mpv_render_context *context = nullptr;
	std::vector<decoderlist_struct> decoderlist;
	fileInfo * fileinfo = nullptr;
	
};

#endif