/*
 *    Copyright (C) 2018
 *    Albrecht Lohofener (albrechtloh@gmx.de)
 *
 *    This file is part of the welle.io.
 *    Many of the ideas as implemented in welle.io are derived from
 *    other work, made available through the GNU general Public License.
 *    All copyrights of the original authors are recognized.
 *
 *    welle.io is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    welle.io is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with welle.io; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef MP2PROCESSOR
#define MP2PROCESSOR

#include <memory>
#include <vector>
#include <cstdint>
#include <cmath>
#include <cstdio>
#include "dab-processor.h"
#include "data/pad_decoder.h"
#include "radio-controller.h"
#include "subchannel_sink.h"
#include "dab_decoder.h"

class Mp2Processor: public DabProcessor, public SubchannelSinkObserver, public PADDecoderObserver
{
    public:
        Mp2Processor(ProgrammeHandlerInterface& mr,
                     int16_t bitRate,
                     const std::string& mp2FileName);
        virtual void addtoFrame(uint8_t *v);

        // SubchannelSinkObserver impl
        void FormatChange(const std::string& /*format*/);
        void StartAudio(int /*samplerate*/, int /*channels*/, bool /*float32*/);
        void PutAudio(const uint8_t* /*data*/, size_t /*len*/);
        void ProcessPAD(const uint8_t* /*xpad_data*/, size_t /*xpad_len*/, bool /*exact_xpad_len*/, const uint8_t* /*fpad_data*/);

        // PADDecoderObserver impl
        virtual void PADChangeDynamicLabel(const DL_STATE& dl);
        virtual void PADChangeSlide(const MOT_FILE& slide);
        virtual void PADLengthError(size_t announced_xpad_len, size_t xpad_len);

    private:
        int16_t bitRate;
        ProgrammeHandlerInterface& myInterface;
        std::unique_ptr<MP2Decoder> mp2Decoder;
        PADDecoder padDecoder;

        struct FILEDeleter{ void operator()(FILE* fd){ if (fd) fclose(fd); }};
        std::unique_ptr<FILE, FILEDeleter> mp2File;

        int audioSamplerate;
        int audioChannels;
        int audioSampleSize;
};
#endif

