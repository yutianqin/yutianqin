package com.example.androidproject

import androidx.lifecycle.ViewModel
import android.content.Context
import android.graphics.Bitmap
import android.media.ThumbnailUtils
import android.util.Log
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.flow.firstOrNull
import kotlinx.coroutines.launch
import java.util.Date

class DrawingDataViewModel(private val repository: DrawingRepository) : ViewModel()
{
    var activeDrawingData: MutableLiveData<DrawingData?> = MutableLiveData()

    private val activeCapturedImage: LiveData<Bitmap?> = MutableLiveData(null)

    val allDrawingData: LiveData<List<DrawingData>> = repository.allDrawingInfo
    fun getActiveCapturedImage(): LiveData<Bitmap?> {
        return activeCapturedImage
    }

    suspend fun setActiveDrawingDataById(id: Int?) {
        repository.setActiveDrawingDataById(id ?: 0)
    }

    private fun addDrawingData(title: String, imageUrl: String?, thumbnail: ByteArray?) {
        val drawingInfo = DrawingData(Date(), Date(), title, imageUrl, thumbnail)
        repository.addNewDrawingInfo(drawingInfo)
    }

    fun setActiveCapturedImage(imageBitmap: Bitmap?) {
        (activeCapturedImage as MutableLiveData).value = imageBitmap
        Log.d("DrawingInfoViewModel", "Bitmap is set as activeCapturedImage.")
    }

    fun addDrawingInfoWithRecentCapturedImage(context: Context): String? {

        val bitmap = activeCapturedImage.value
        if (bitmap == null) {
            Log.d("DrawingInfoViewModel", "Bitmap is null.")
            return null
        }

        if (activeDrawingData.value == null) {
            val title = "Untitled"


            val imagePath = saveImage(bitmap, context)
            if (imagePath == null) {
                Log.d("DrawingInfoViewModel", "Image path is null.")
                return null
            }

            val thumbnail = ThumbnailUtils.extractThumbnail(bitmap, 256, 256)
            addDrawingData(title, imagePath, bitmapToByteArray(thumbnail))
            return imagePath
        } else {
            val imagePath =
                overwriteCurrentImageFile(bitmap, context, activeDrawingData.value?.imagePath ?: "")
            if (imagePath == null) {
                Log.d("DrawingInfoViewModel", "Image path is null.")
                return null
            }

            val thumbnail = ThumbnailUtils.extractThumbnail(bitmap, 256, 256)
            updateThumbnailForActiveDrawingInfo(bitmapToByteArray(thumbnail))

            return imagePath
        }
    }

    private fun updateThumbnailForActiveDrawingInfo(thumbnail: ByteArray) {
        repository.updateDrawingInfoThumbnail(thumbnail, activeDrawingData.value?.id ?: 0)
    }

    fun updateDrawingTitle(newTitle: String) {
        val activeDrawingId = activeDrawingData.value?.id ?: return
        repository.updateDrawingTitle(newTitle, activeDrawingId)
    }

    suspend fun setActiveDrawingDataById(id: Int) {
        viewModelScope.launch {
            val drawingData = repository.getDrawingDataById(id).firstOrNull()
            activeDrawingData.postValue(drawingData)
        }
    }
}

class DrawingInfoViewModelFactory(private val repository: DrawingRepository) :
    ViewModelProvider.Factory {
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(DrawingDataViewModel::class.java)) {
            @Suppress("UNCHECKED_CAST")
            return DrawingDataViewModel(repository) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}