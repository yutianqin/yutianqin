package com.example.androidproject

import android.content.Context
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Path
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import java.io.ByteArrayOutputStream
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

class CustomView(context: Context, attrs: AttributeSet? = null) : View(context, attrs) {
    private val paint = Paint().apply {
        color = Color.BLACK
        strokeWidth = currentBrushSize
        style = Paint.Style.STROKE
    }
    private var path = Path()
    private var drawingBitmap: Bitmap? = null
    var viewModel: DrawingViewModel? = null

    private var currentBrushType: BrushPickerFragment.BrushType = BrushPickerFragment.BrushType.NORMAL
    private var currentBrushSize: Float = 5f
    private var offset: Float = 15F

    fun setBitmap(bitmap: Bitmap?) {
        drawingBitmap = bitmap
        invalidate()
    }

    fun attachTouchListener() {
        setOnTouchListener { _, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    when (currentBrushType) {
                        BrushPickerFragment.BrushType.NORMAL -> {
                            path.moveTo(event.x, event.y)
                        }

                        BrushPickerFragment.BrushType.CIRCLE -> {
                            path.addCircle(event.x, event.y, offset / 2, Path.Direction.CW)
                        }

                        BrushPickerFragment.BrushType.SQUARE -> {
                            val left = event.x - offset / 2
                            val top = event.y - offset / 2
                            val right = event.x + offset / 2
                            val bottom = event.y + offset / 2
                            path.addRect(left, top, right, bottom, Path.Direction.CW)
                        }
                    }
                    invalidate()
                    true
                }

                MotionEvent.ACTION_MOVE -> {
                    when (currentBrushType) {
                        BrushPickerFragment.BrushType.NORMAL -> {
                            path.lineTo(event.x, event.y)
                        }

                        BrushPickerFragment.BrushType.CIRCLE -> {
                            path.addCircle(event.x, event.y, offset / 2, Path.Direction.CW)
                        }

                        BrushPickerFragment.BrushType.SQUARE -> {
                            val left = event.x - offset / 2
                            val top = event.y - offset / 2
                            val right = event.x + offset / 2
                            val bottom = event.y + offset / 2
                            path.addRect(left, top, right, bottom, Path.Direction.CW)
                        }
                    }
                    updateBitmap()
                    invalidate()
                    true
                }
                MotionEvent.ACTION_UP -> {
                    when (currentBrushType) {
                        BrushPickerFragment.BrushType.NORMAL -> {
                        }

                        BrushPickerFragment.BrushType.CIRCLE -> {
                            path.addCircle(event.x, event.y, offset / 2, Path.Direction.CW)
                        }

                        BrushPickerFragment.BrushType.SQUARE -> {
                            val left = event.x - offset / 2
                            val top = event.y - offset / 2
                            val right = event.x + offset / 2
                            val bottom = event.y + offset / 2
                            path.addRect(left, top, right, bottom, Path.Direction.CW)
                        }
                    }
                    updateBitmap() // Save the current path to the bitmap
                    path.reset() // Reset the path so it's ready for new drawings
                    viewModel?.drawingBitmap?.value = this.getDrawingBitmap() // Safely accessing viewModel
                    performClick() // Not actually used, just adding so I don't get warnings
                    true
                }
                else -> false
            }
        }
    }

    // New function to change the brush type
    fun changeBrushType(brushType: BrushPickerFragment.BrushType) {
        currentBrushType = brushType
        invalidate()
    }

    // New function to change the brush size
    fun changeBrushSize(size: Float) {
        currentBrushSize = size
        paint.strokeWidth = size
        invalidate()
    }

    // Not actually used, just adding so I don't get warnings
    override fun performClick(): Boolean {
        super.performClick()
        return true
    }

    private fun updateBitmap() {
        if (drawingBitmap == null) {
            drawingBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        }
        val tempCanvas = Canvas(drawingBitmap!!)
        tempCanvas.drawPath(path, paint)
    }

    override fun onDraw(canvas: Canvas) {

        super.onDraw(canvas)
        drawingBitmap?.let {
            canvas?.drawBitmap(it, 0f, 0f, null)
        }

    }

    fun getDrawingBitmap(): Bitmap? {
        return drawingBitmap
    }

    fun changePaintColor(color: Int) {
        paint.color = color
        invalidate()
    }
    val viewWidth: Int
        get() = width // The get() method returns the width of this view.

    val viewHeight: Int
        get() = height // The get() method returns the height of this view.

}