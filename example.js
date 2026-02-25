const createRuckigModule = require('./dist/ruckig.js');

createRuckigModule().then(function(Module) {
    var dt = 0.001; // 1ms timestep
    var joint = new Module.RuckigJoint(dt);

    joint.setConstraints(180, 360, 1000); // deg/s, deg/s², deg/s³
    joint.setTargetPosition(90);

    console.log('Moving to 90°...');
    var result;
    do {
        result = joint.update();
    } while (result === 0);

    console.log('Done in ' + joint.getTime().toFixed(3) + 's');
    console.log('Position: ' + joint.getPosition().toFixed(2) + '°');
    console.log('Velocity: ' + joint.getVelocity().toFixed(2) + '°/s');

    joint.delete();
});
