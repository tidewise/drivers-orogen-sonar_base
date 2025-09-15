/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "SonarToFrameTask.hpp"
#include <opencv2/imgproc/imgproc.hpp>

using namespace sonar_base;
using namespace base::samples::frame;
using namespace cv;

SonarToFrameTask::SonarToFrameTask(std::string const& name)
    : SonarToFrameTaskBase(name)
{
}

SonarToFrameTask::~SonarToFrameTask()
{
}

bool SonarToFrameTask::configureHook()
{
    if (!SonarToFrameTaskBase::configureHook()) {
        return false;
    }
    m_window_size = _window_size.get();
    m_image = Mat::zeros(m_window_size, m_window_size, CV_8UC3);
    m_grayscale_image = Mat::zeros(m_window_size, m_window_size, CV_8UC1);

    Frame* frame = new Frame(m_window_size,
        m_window_size,
        8U,
        base::samples::frame::frame_mode_t::MODE_GRAYSCALE,
        0,
        0);
    m_output_frame.reset(frame);
    return true;
}

bool SonarToFrameTask::startHook()
{
    if (!SonarToFrameTaskBase::startHook())
        return false;
    return true;
}

void SonarToFrameTask::updateHook()
{
    SonarToFrameTaskBase::updateHook();
    base::samples::Sonar sonar;
    if (_sonar.read(sonar) != RTT::NewData) {
        return;
    }
    configureLUT(sonar);
    setImage(sonar);
    outputFrame();
}

void SonarToFrameTask::configureLUT(base::samples::Sonar const& sonar)
{
    if (!(m_lut && m_lut->hasMatchingConfiguration(sonar, m_window_size))) {
        m_lut = std::make_unique<SonarToImageLUT>(sonar, m_window_size);
    }
}

void SonarToFrameTask::setImage(base::samples::Sonar const& sonar)
{
    m_image = 0;
    for (unsigned int idx = 0; idx < sonar.bins.size(); idx++) {
        m_lut->updateImage(m_image,
            idx,
            sonar.bins[idx] * 255,
            sonar.bin_count);
    }
    cv::cvtColor(m_image, m_grayscale_image, cv::COLOR_BGR2GRAY);
}

void SonarToFrameTask::outputFrame()
{
    Frame* out_frame = m_output_frame.write_access();
    out_frame->time = base::Time::now();
    out_frame->received_time = out_frame->time;
    out_frame->setImage(m_grayscale_image.data,
        m_grayscale_image.total() * m_grayscale_image.elemSize());
    out_frame->setStatus(STATUS_VALID);
    m_output_frame.reset(out_frame);
    _frame.write(m_output_frame);
}

void SonarToFrameTask::errorHook()
{
    SonarToFrameTaskBase::errorHook();
}

void SonarToFrameTask::stopHook()
{
    SonarToFrameTaskBase::stopHook();
}

void SonarToFrameTask::cleanupHook()
{
    SonarToFrameTaskBase::cleanupHook();
}
