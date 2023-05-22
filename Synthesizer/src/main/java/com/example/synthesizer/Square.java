package com.example.synthesizer;

public class Square implements AudioComponent{

    AudioComponent input_;
    double freq;
    public Square( double frequency ) {
        freq = frequency;
    }
    public void setFreq( double frequency ) {
        this.freq = frequency;
    }
    @Override
    public AudioClip getClip() {
        AudioClip ac = new AudioClip();
        int count = 0;
        int var = 0;
        for(int i = 0; i < AudioClip.TOTAL_SAMPLE; i++) {
            ac.setSample(count++, (short)(var * ( 2 * freq * i / AudioClip.sampleRate )));
            if ((double)(freq * i / AudioClip.sampleRate) % 1 > 0.5) {
                var = Short.MAX_VALUE;
            } else {
                var = Short.MIN_VALUE;
            }
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
