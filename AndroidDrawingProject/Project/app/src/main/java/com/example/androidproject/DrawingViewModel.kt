package com.example.androidproject

import android.graphics.Bitmap
import android.graphics.Color
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.launch

open class DrawingViewModel : ViewModel() {

    open val drawingBitmap: MutableLiveData<Bitmap> = MutableLiveData()

    val currentColor: MutableLiveData<Int> = MutableLiveData(Color.BLACK)

    var currentBrushType: MutableLiveData<BrushPickerFragment.BrushType> = MutableLiveData(BrushPickerFragment.BrushType.NORMAL)
    var currentPenSize: MutableLiveData<Float> = MutableLiveData(5f)

    fun changeColor(color: Int) {
        currentColor.value = color
    }

    fun changePenType(brushType: BrushPickerFragment.BrushType) {
        currentBrushType.value = brushType
    }

    fun changePenSize(size: Float) {
        currentPenSize.value = size
    }
}