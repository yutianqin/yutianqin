package com.example.synthesizer;
import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;

public class Mn {
    public static void main(String[] args) throws LineUnavailableException {

        Clip c = AudioSystem.getClip();

        AudioFormat format16 = new AudioFormat(44100, 16, 1, true, false);

        AudioComponent gen1 = new SineWave(440);
        AudioComponent gen2 = new SineWave(800);

//        Filter filter = new Filter(1);
//        filter.connectInput(gen1);
//        AudioClip clip = filter.getClip();

//        Mixer mixer = new Mixer();
//        mixer.connectInput(gen1);
//        mixer.connectInput(gen2);
//        AudioClip clip = mixer.getClip();

//        Square square = new Square(220);
//        square.connectInput(gen1);
//        AudioClip clip = square.getClip();

//        WhiteNoise wn = new WhiteNoise();
//        AudioClip clip = wn.getClip();

        linearRamp lr = new linearRamp(50,2000);
        VFSineWave vfw = new VFSineWave();
        vfw.connectInput(lr.getClip());

        AudioClip clip = vfw.getClip();





        c.open( format16, clip.getData(), 0, clip.getData().length );
        System.out.println("About to play...");
        c.start();
        c.loop( 1 );
        while( c.getFramePosition() < AudioClip.TOTAL_SAMPLE || c.isActive() || c.isRunning() ){
            // Do nothing while we wait for the note to play.
        }
        System.out.println("Done");
        c.close();
    }
}
