package com.example.synthesizer;
import java.util.Random;


public class WhiteNoise implements AudioComponent{
    @Override
    public AudioClip getClip() {
        AudioClip ac = new AudioClip();
        Random randNum = new Random();
        for ( int i = 0; i < AudioClip.TOTAL_SAMPLE; i++ ) {
            ac.setSample(i, (short) randNum.nextInt(1 << 16));
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
