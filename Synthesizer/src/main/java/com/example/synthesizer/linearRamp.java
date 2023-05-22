package com.example.synthesizer;

public class linearRamp implements AudioComponent{
    float start;
    float stop;
    public linearRamp(float sta, float sto) {
        start = sta;
        stop = sto;
    }

    @Override
    public AudioClip getClip() {
        AudioClip ac = new AudioClip();
        for ( int i = 0; i < AudioClip.TOTAL_SAMPLE; i++ ) {
            ac.setSample(i, (short) (( start * ( AudioClip.TOTAL_SAMPLE - i ) + stop * i)
                    / AudioClip.TOTAL_SAMPLE));
        }
        return ac;
    }

    @Override
    public boolean hasInput() {
        return false;
    }

    @Override
    public void connectInput(AudioComponent input) {

    }
}
