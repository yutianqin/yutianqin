package com.example.synthesizer;

public class SineWave implements AudioComponent{
    double freq;
    public SineWave( double frequency ) {
        freq = frequency;
    }

    public void setFreq( double frequency ) {
        this.freq = frequency;
    }


    @Override
    public AudioClip getClip() {
        AudioClip ac = new AudioClip();
        int count = 0;
        for (int i = 0; i < AudioClip.TOTAL_SAMPLE; i++) {
            ac.setSample(count,
                    (short)(Short.MAX_VALUE * Math.sin( 2 * Math.PI * freq * i / AudioClip.TOTAL_SAMPLE )));
            count++;
        }
        return ac;
    }

    @Override
    public boolean hasInput() {
        return getClip() != null;
    }

    @Override
    public void connectInput(AudioComponent input) {
    }
}
