package com.example.synthesizer;

public class VFSineWave implements AudioComponent{
    AudioComponent input_;

    public VFSineWave() {
    }

    @Override
    public AudioClip getClip() {
//        AudioClip inpt = input_.getClip();
        AudioClip ac = new AudioClip();
        double phase = 0;
        for(int i = 0; i < AudioClip.TOTAL_SAMPLE; i++) {
            phase += 2 * Math.PI * input_.getClip().getSample(i) / AudioClip.sampleRate;
            ac.setSample(i, (short)(Short.MAX_VALUE * Math.sin(phase)));
        }
        return ac;
    }

    @Override
    public boolean hasInput() {
        return false;
    }

    @Override
    public void connectInput(AudioComponent input) {
        input_ = input;
    }
}
