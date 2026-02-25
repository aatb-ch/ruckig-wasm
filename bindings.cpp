#include <emscripten/bind.h>
#include <ruckig/ruckig.hpp>
#include <array>

using namespace emscripten;
using namespace ruckig;

// Simple wrapper around Ruckig<1> for single-axis servo control
class RuckigJoint {
    Ruckig<1> otg;
    InputParameter<1> input;
    OutputParameter<1> output;
    int last_result {1};

public:
    explicit RuckigJoint(double delta_time) : otg(delta_time) {
        input.current_position = {0.0};
        input.current_velocity = {0.0};
        input.current_acceleration = {0.0};
        input.target_position = {0.0};
        input.target_velocity = {0.0};
        input.target_acceleration = {0.0};
        input.max_velocity = {180.0};
        input.max_acceleration = {360.0};
        input.max_jerk = {1000.0};
        input.control_interface = ControlInterface::Position;
    }

    // Set kinematic constraints
    void setConstraints(double max_vel, double max_accel, double max_jerk) {
        input.max_velocity = {max_vel};
        input.max_acceleration = {max_accel};
        input.max_jerk = {max_jerk};
    }

    // Set target position (position control mode)
    void setTargetPosition(double position) {
        input.control_interface = ControlInterface::Position;
        input.target_position = {position};
        input.target_velocity = {0.0};
        input.target_acceleration = {0.0};
        last_result = 0;
    }

    // Set target velocity (velocity/jog control mode)
    void setTargetVelocity(double velocity) {
        input.control_interface = ControlInterface::Velocity;
        input.target_velocity = {velocity};
        input.target_acceleration = {0.0};
        last_result = 0;
    }

    // Stop (decelerate to zero velocity using velocity control)
    void stop() {
        input.control_interface = ControlInterface::Velocity;
        input.target_velocity = {0.0};
        input.target_acceleration = {0.0};
        last_result = 0;
    }

    // Step the trajectory by one delta_time
    // Returns: 0 = working, 1 = finished, <0 = error
    int update() {
        Result result = otg.update(input, output);
        output.pass_to_input(input);
        return static_cast<int>(result);
    }

    // Compute full trajectory offline and sample it at given time
    // Useful for plotting the entire trajectory at once
    void computeTrajectory() {
        Trajectory<1> trajectory;
        otg.calculate(input, trajectory);
    }

    double getPosition() const { return output.new_position[0]; }
    double getVelocity() const { return output.new_velocity[0]; }
    double getAcceleration() const { return output.new_acceleration[0]; }
    double getJerk() const { return output.new_jerk[0]; }
    double getTime() const { return output.time; }
    double getDuration() const { return output.trajectory.get_duration(); }
    bool isRunning() const { return last_result == 0; }

    // Get the current state
    double getCurrentPosition() const { return input.current_position[0]; }
    double getCurrentVelocity() const { return input.current_velocity[0]; }
    double getCurrentAcceleration() const { return input.current_acceleration[0]; }

    // Force-set current state (e.g. for initialization)
    void setCurrentState(double pos, double vel, double accel) {
        input.current_position = {pos};
        input.current_velocity = {vel};
        input.current_acceleration = {accel};
        otg.reset();
    }
};

EMSCRIPTEN_BINDINGS(ruckig_wasm) {
    class_<RuckigJoint>("RuckigJoint")
        .constructor<double>()
        .function("setConstraints", &RuckigJoint::setConstraints)
        .function("setTargetPosition", &RuckigJoint::setTargetPosition)
        .function("setTargetVelocity", &RuckigJoint::setTargetVelocity)
        .function("stop", &RuckigJoint::stop)
        .function("update", &RuckigJoint::update)
        .function("getPosition", &RuckigJoint::getPosition)
        .function("getVelocity", &RuckigJoint::getVelocity)
        .function("getAcceleration", &RuckigJoint::getAcceleration)
        .function("getJerk", &RuckigJoint::getJerk)
        .function("getTime", &RuckigJoint::getTime)
        .function("getDuration", &RuckigJoint::getDuration)
        .function("isRunning", &RuckigJoint::isRunning)
        .function("getCurrentPosition", &RuckigJoint::getCurrentPosition)
        .function("getCurrentVelocity", &RuckigJoint::getCurrentVelocity)
        .function("getCurrentAcceleration", &RuckigJoint::getCurrentAcceleration)
        .function("setCurrentState", &RuckigJoint::setCurrentState)
        ;
}
