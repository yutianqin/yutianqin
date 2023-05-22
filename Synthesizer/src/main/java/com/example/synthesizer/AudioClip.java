package com.example.synthesizer;

import java.util.Arrays;



public class AudioClip implements AudioComponent {
    static final int duration = 2;
    static int sampleRate = 44100;
    public static int TOTAL_SAMPLE = sampleRate * duration;
    public int numByte = TOTAL_SAMPLE * 2;
    public byte[] byteArray;

    public AudioClip() {
        byteArray = new byte[numByte];
        Arrays.fill(byteArray,(byte)0);
    }

    public short getSample( int index ) {
        int result = (short) (byteArray[2*index + 1] << 8) | (byteArray[2*index] & 0xFF) ;
        return (short)result;
    }

    public void setSample( int index, short value ) {
        byteArray[index*2] = (byte) value;
        byteArray[index*2+1] = (byte) (value >> 8);
    }

    public byte[] getData() {
        return Arrays.copyOf(byteArray, byteArray.length);
    }


    @Override
    public AudioClip getClip() {
        return this;
    }
    @Override
    public boolean hasInput() {
        return byteArray != null;
    }
    @Override
    public void connectInput(AudioComponent input) {
    }

}
