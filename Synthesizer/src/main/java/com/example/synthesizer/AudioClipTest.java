package com.example.synthesizer;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class AudioClipTest {

    @Test
    void Test() {
        AudioClip a = new AudioClip();
        int x = 0;
        for (short i = -32768; i < 32767; i++ ) {
            a.setSample(x++, i);
            if ( a.getSample(x) != i ) {
                System.out.println("Bytes at the " + x
                        +" and " + x+1 + " are not equals to "+ i);
                break;
            }
        }



    }

}