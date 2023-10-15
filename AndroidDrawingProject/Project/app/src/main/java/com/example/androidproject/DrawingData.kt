package com.example.androidproject

import androidx.room.Entity
import androidx.room.PrimaryKey
import androidx.room.TypeConverter
import java.io.Serializable
import java.util.Date


//Room can not handle Date objects directly so we need type converters
class Converters {
    @TypeConverter
    fun fromTimestamp(value: Long?): Date? {
        return value?.let { Date(it) }
    }

    @TypeConverter
    fun dateToTimestamp(date: Date?): Long? {
        return date?.time?.toLong()
    }
}

//Defines a SQLITE table
@Entity
data class DrawingData(
    var lastModifiedDate: Date,
    var createdDate: Date,
    var drawingTitle: String,
    var imagePath: String?, // Make imageUrl nullable
    var thumbnail: ByteArray?
) : Serializable {
    @PrimaryKey(autoGenerate = true)
    var id: Int = 0 // integer primary key for the DB

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as DrawingData

        if (lastModifiedDate != other.lastModifiedDate) return false
        if (createdDate != other.createdDate) return false
        if (drawingTitle != other.drawingTitle) return false
        if (imagePath != other.imagePath) return false
        if (thumbnail != null) {
            if (other.thumbnail == null) return false
            if (!thumbnail.contentEquals(other.thumbnail)) return false
        } else if (other.thumbnail != null) return false
        if (id != other.id) return false

        return true
    }

    override fun hashCode(): Int {
        var result = lastModifiedDate.hashCode()
        result = 31 * result + createdDate.hashCode()
        result = 31 * result + drawingTitle.hashCode()
        result = 31 * result + (imagePath?.hashCode() ?: 0)
        result = 31 * result + (thumbnail?.contentHashCode() ?: 0)
        result = 31 * result + id
        return result
    }
}