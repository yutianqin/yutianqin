package com.example.androidproject

import android.graphics.Bitmap
import android.graphics.Color
import android.graphics.drawable.ColorDrawable
import android.os.Bundle
import android.view.Gravity
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.PopupWindow
import androidx.lifecycle.Observer
//import androidx.navigation.fragment.findNavController
import android.widget.ImageButton
import androidx.fragment.app.activityViewModels
import androidx.fragment.app.viewModels
import androidx.lifecycle.lifecycleScope
import java.io.ByteArrayOutputStream
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

class DrawFragment : Fragment(R.layout.fragment_draw) {
    private val drawingViewModel: DrawingViewModel by activityViewModels()
    private lateinit var application: DrawingApp
    private lateinit var drawingRepository: DrawingRepository


    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        application = requireActivity().application as DrawingApp
        drawingRepository = application.DrawingRepository

        val customView = view.findViewById<CustomView>(R.id.custom_view)

        // Set the viewModel for the customView
        customView.viewModel = drawingViewModel

        customView.attachTouchListener()

        // Observe changes in the drawing Bitmap
        drawingViewModel.drawingBitmap.observe(viewLifecycleOwner, Observer { bitmap ->
            customView.setBitmap(bitmap)
        })

        // Observe changes in the currentColor from ViewModel and update CustomView's paint color
        drawingViewModel.currentColor.observe(viewLifecycleOwner, Observer { color ->
            customView.changePaintColor(color)
        })

        drawingViewModel.currentBrushType.observe(viewLifecycleOwner, Observer {brushType ->
            customView.changeBrushType(brushType) ;
        })

        drawingViewModel.currentPenSize.observe(viewLifecycleOwner, Observer { brushSize->
            customView.changeBrushSize(brushSize) ;
        })

        // Observe changes in the drawing Bitmap from shared ViewModel
        drawingViewModel.drawingBitmap.observe(viewLifecycleOwner, Observer { bitmap ->
            bitmap?.let {
                // set the bitmap to your CustomView here
                customView.setBitmap(it)
            }
        })

        val colorPickerButton: Button = view.findViewById(R.id.colorPickerButton)
        colorPickerButton.setOnClickListener {
            showColorPickerPopup(it)
        }

        val brushPickerButton: Button = view.findViewById(R.id.brushPickerButton)
        brushPickerButton.setOnClickListener {
            showBrushPickerPopup(it)
        }

        val saveButton: Button = view.findViewById(R.id.saveButton)
        saveButton.setOnClickListener {
            saveDrawing()
        }
    }
    fun showColorPickerPopup(anchorView: View) {
        val inflater = LayoutInflater.from(context)
        val popupView = inflater.inflate(R.layout.fragment_color_picker, null)

        // Initialize the buttons and set their click listeners right here
        val redButton: Button = popupView.findViewById(R.id.redButton)
        val blueButton: Button = popupView.findViewById(R.id.blueButton)
        val greenButton: Button = popupView.findViewById(R.id.greenButton)
        val yellowButton: Button = popupView.findViewById(R.id.yellowButton)
        val blackButton: Button = popupView.findViewById(R.id.blackButton)

        redButton.setOnClickListener { drawingViewModel.changeColor(Color.RED) }
        blueButton.setOnClickListener { drawingViewModel.changeColor(Color.BLUE) }
        greenButton.setOnClickListener { drawingViewModel.changeColor(Color.GREEN) }
        yellowButton.setOnClickListener { drawingViewModel.changeColor(Color.YELLOW) }
        blackButton.setOnClickListener { drawingViewModel.changeColor(Color.BLACK) }

        val popupWindow = PopupWindow(
            popupView,
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT
        )

        // Measure the content view to set width and height explicitly
        popupView.measure(View.MeasureSpec.UNSPECIFIED, View.MeasureSpec.UNSPECIFIED)
        popupWindow.width = popupView.measuredWidth
        popupWindow.height = popupView.measuredHeight

        popupWindow.isFocusable = true
        popupWindow.setBackgroundDrawable(ColorDrawable(Color.TRANSPARENT))
        popupWindow.isOutsideTouchable = true

        popupWindow.showAtLocation(anchorView, Gravity.TOP or Gravity.CENTER_HORIZONTAL, 0, 0)
    }

    private fun showBrushPickerPopup(anchorView: View) {
        val inflater = LayoutInflater.from(context)
        val popupView = inflater.inflate(R.layout.fragment_brush_picker, null)

        val normalPenButton: ImageButton = popupView.findViewById(R.id.normalBrush)
        val circlePenButton: ImageButton = popupView.findViewById(R.id.circleBrush)
        val squarePenButton: ImageButton = popupView.findViewById(R.id.squareBrush)
        val sizeButton1: Button = popupView.findViewById(R.id.size1)
        val sizeButton2: Button = popupView.findViewById(R.id.size2)
        val sizeButton3: Button = popupView.findViewById(R.id.size3)

        normalPenButton.setOnClickListener {
            drawingViewModel.changePenType(BrushPickerFragment.BrushType.NORMAL)
        }

        circlePenButton.setOnClickListener {
            drawingViewModel.changePenType(BrushPickerFragment.BrushType.CIRCLE)
        }

        squarePenButton.setOnClickListener {
            drawingViewModel.changePenType(BrushPickerFragment.BrushType.SQUARE)
        }

        sizeButton1.setOnClickListener {
            drawingViewModel.changePenSize(5f)
        }

        sizeButton2.setOnClickListener {
            drawingViewModel.changePenSize(10f)
        }

        sizeButton3.setOnClickListener {
            drawingViewModel.changePenSize(15f)
        }

        val popupWindow = PopupWindow(
            popupView,
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT
        )

        popupView.measure(View.MeasureSpec.UNSPECIFIED, View.MeasureSpec.UNSPECIFIED)
        popupWindow.width = popupView.measuredWidth
        popupWindow.height = popupView.measuredHeight

        popupWindow.isFocusable = true
        popupWindow.setBackgroundDrawable(ColorDrawable(Color.TRANSPARENT))
        popupWindow.isOutsideTouchable = true

        popupWindow.showAtLocation(anchorView, Gravity.TOP or Gravity.LEFT, 0, 0)
    }

    override fun onPause() {
        super.onPause()
        // When fragment goes to the background, save the current drawing state to ViewModel
        val customView = view?.findViewById<CustomView>(R.id.custom_view)
        drawingViewModel.drawingBitmap.value = customView?.getDrawingBitmap()
    }

    private fun saveDrawing() {
        val customView = view?.findViewById<CustomView>(R.id.custom_view)
        val bitmap = customView?.getDrawingBitmap()!!

        val stream = ByteArrayOutputStream()
        bitmap.compress(Bitmap.CompressFormat.PNG, 100, stream)
        val byteArray = stream.toByteArray()

        // Get current date and time
        val currentDate = Date()

        // Create a SimpleDateFormat object with the desired format
        val formatter = SimpleDateFormat("yyyyMMdd_HHmmss", Locale.getDefault())

        // Format the current date and time to the string representing the desired format
        val formattedDateString = formatter.format(currentDate)

        // Use formatted string as drawing title
        val drawingTitle = "Drawing_$formattedDateString"

        val drawingData = DrawingData(
            lastModifiedDate = currentDate,
            createdDate = currentDate,
            drawingTitle = drawingTitle,
            imagePath = null,
            thumbnail = byteArray
        )

        drawingRepository.addNewDrawingInfo(drawingData)
    }
}