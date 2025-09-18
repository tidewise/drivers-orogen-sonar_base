# frozen_string_literal: true

using_task_library "sonar_base"

describe OroGen.sonar_base.SonarToFrameTask do
    run_live

    attr_reader :task

    before do
        @task = syskit_deploy(
            OroGen.sonar_base.SonarToFrameTask.deployed_as("task_test")
        )
        task.properties.window_size = 512
        syskit_configure_and_start(@task)
    end

    it "outputs a frame" do
        sonar = Types.base.samples.Sonar.new(
            bins: [1, 2, 3, 4, 5, 6],
            beam_count: 3,
            bin_count: 2,
            beam_width: Types.base.Angle.new(rad: 0.1),
            bin_duration: Time.at(1),
            speed_of_sound: 1,
            bearings: [
                Types.base.Angle.new(rad: -0.1),
                Types.base.Angle.new(rad: 0),
                Types.base.Angle.new(rad: 0.1)
            ]
        )

        expect_execution do
            syskit_write @task.sonar_port, sonar
        end.to do
            have_one_new_sample(task.frame_port)    
        end
    end
end
