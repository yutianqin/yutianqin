package com.example.synthesizer;

public class Filter implements AudioComponent{
    double scale;
    AudioComponent input_;
    public Filter(double paraScale) {
        scale = paraScale;
    }

    @Override
    public AudioClip getClip() {
        AudioClip original = input_.getClip();
        AudioClip adjustClip = new AudioClip();
        for ( int i = 0; i < AudioClip.TOTAL_SAMPLE; i++) {
            // Clamping
            if (adjustClip.getSample(i)*scale > Short.MAX_VALUE) {
                adjustClip.setSample(i, Short.MAX_VALUE);
            }
            else if (adjustClip.getSample(i)*scale < Short.MIN_VALUE) {
                adjustClip.setSample(i, Short.MIN_VALUE);
            }
            else adjustClip.setSample(i, (short)(scale * original.getSample(i)));
        }
        return adjustClip;
    }

    @Override
    public boolean hasInput() {
        return true;
    }

    @Override
    public void connectInput(AudioComponent input) {
        input_ = input;
    }
}
