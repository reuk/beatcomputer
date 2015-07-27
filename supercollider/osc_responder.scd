s.boot;

(
b = NetAddr("127.0.0.1", 7000);
SystemClock.sched(0.0, {b.sendMsg("/time_server", "tick"); 0.01});

SynthDef(\bd, {|gate=1, freq=100|
	freq = freq % 100 + 100;
	Out.ar([0, 1], SinOsc.ar(freq + EnvGen.ar(Env.perc(releaseTime:0.02, level:freq))) * EnvGen.kr(Env.perc(releaseTime: 0.8), gate, doneAction: 2));
}).add;

SynthDef(\string, {|freq=100|
	freq = freq % 500 + 100;
	Out.ar([0, 1], Pluck.ar(WhiteNoise.ar(0.1), Impulse.ar(freq / 128), freq.reciprocal, freq.reciprocal, 10)) * EnvGen.ar(Env.perc(releaseTime: 0.2), doneAction: 2);}).add;

p = OSCFunc({|msg| Synth(\string, [\freq, msg[2]]); Synth(\bd, [\freq, msg[2]]);}, '/beatcomputer', nil, 7001);
)