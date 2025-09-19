/* Generated from orogen/lib/orogen/templates/tasks/Task.hpp */

#ifndef SONAR_BASE_SONARTOFRAMETASK_TASK_HPP
#define SONAR_BASE_SONARTOFRAMETASK_TASK_HPP

#include "sonar_base/SonarToFrameTaskBase.hpp"
#include "sonar_base/SonarToImageLUT.hpp"

namespace sonar_base {
    class SonarToFrameTask : public SonarToFrameTaskBase {
        friend class SonarToFrameTaskBase;

    private:
        std::unique_ptr<SonarToImageLUT> m_lut;
        cv::Mat m_image;
        cv::Mat m_grayscale_image;
        RTT::extras::ReadOnlyPointer<base::samples::frame::Frame> m_output_frame;
        int m_window_size = 0;

        void setImage(base::samples::Sonar const& sonar);
        void outputFrame();
        bool reconfigureLUT(base::samples::Sonar const& sonar);
        void configureImagesAndFrame();

    public:
        SonarToFrameTask(std::string const& name = "sonar_base::SonarToFrameTask");
        ~SonarToFrameTask();
        bool configureHook();
        bool startHook();
        void updateHook();
        void errorHook();
        void stopHook();
        void cleanupHook();
    };
}

#endif
