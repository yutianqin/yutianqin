package com.example.synthesizer;
import java.util.ArrayList;

public class Mixer implements AudioComponent {

    ArrayList<AudioComponent> allClips= new ArrayList<>();

    @Override
    public AudioClip getClip() {
        AudioClip mix = new AudioClip();
        for ( int x = 0; x < allClips.size(); x++ ) {
            AudioClip temp = allClips.get(x).getClip();
            for( int i = 0; i < AudioClip.TOTAL_SAMPLE; i++ ) {
                mix.setSample( i, (short) (temp.getSample(i) + mix.getSample(i)/allClips.size()) );
            }
        }
        return mix;
    }

    @Override
    public boolean hasInput() {
        return true;
    }

    @Override
    public void connectInput(AudioComponent input) {
        allClips.add(input);
    }


}
