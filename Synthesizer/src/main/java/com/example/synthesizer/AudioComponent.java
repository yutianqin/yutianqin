package com.example.synthesizer;


public interface AudioComponent {
    AudioClip getClip();
    default boolean hasInput() { return false; }
    default void connectInput(AudioComponent input) { assert( false ); }
    default void removeInput(AudioComponent input) { assert( false ); };
}