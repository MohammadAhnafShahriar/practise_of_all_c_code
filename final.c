#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <"iup.h">

// ==========================================
// 1. DATA STRUCTURES
// ==========================================

#pragma pack(push, 1)
typedef struct {
    uint16_t type;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t pixelOffset;
} BMPFileHeader;

typedef struct {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    char temp[24];
} BMPInfoHeader;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} Pixel;
#pragma pack(pop) // Moved here so Pixel stays exactly 3 bytes

typedef struct {
    int width;
    int height;
    Pixel *data; 
} Image;

// ==========================================
// 2. MEMORY MANAGEMENT & UTILITIES
// ==========================================

Image* createImage(int width, int height) {
    Image *img = (Image*)malloc(sizeof(Image));
    img->width = width;
    img->height = height;
    img->data = (Pixel*)malloc(width * height * sizeof(Pixel));
    return img;
}

void freeImage(Image *img) {
    if (img) {
        if (img->data) free(img->data);
        free(img);
    }
}

Image* copyImage(Image *src) {
    if (!src) return NULL;
    Image *dst = createImage(src->width, src->height);
    memcpy(dst->data, src->data, src->width * src->height * sizeof(Pixel));
    return dst;
}

uint8_t clamp(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return (uint8_t)value;
}

// ==========================================
// 3. FILE I/O
// ==========================================

Image* loadBMP(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;
    fread(&fileHeader, sizeof(BMPFileHeader), 1, fp);
    fread(&infoHeader, sizeof(BMPInfoHeader), 1, fp);

    if (fileHeader.type != 0x4D42 || infoHeader.bitsPerPixel != 24) {
        fclose(fp);
        return NULL; 
    }

    int width = infoHeader.width;
    int height = abs(infoHeader.height);
    int padding = (4 - (width * sizeof(Pixel)) % 4) % 4;

    Image *img = createImage(width, height);
    fseek(fp, fileHeader.pixelOffset, SEEK_SET);

    for (int y = height - 1; y >= 0; y--) { 
        // Read the entire row at once for better performance
        fread(&img->data[y * width], sizeof(Pixel), width, fp);
        fseek(fp, padding, SEEK_CUR);
    }

    fclose(fp);
    return img;
}

int saveBMP(const char *filename, Image *img) {
    if (!img) return 0;
    FILE *out = fopen(filename, "wb");
    if (!out) return 0;

    int padding = (4 - (img->width * sizeof(Pixel)) % 4) % 4;
    int rowSize = img->width * sizeof(Pixel) + padding;
    int dataSize = rowSize * img->height;

    BMPFileHeader fileHeader = {0x4D42, 54 + dataSize, 0, 0, 54};
    BMPInfoHeader infoHeader = {40, img->width, img->height, 1, 24, {0}};

    fwrite(&fileHeader, sizeof(BMPFileHeader), 1, out);
    fwrite(&infoHeader, sizeof(BMPInfoHeader), 1, out);

    uint8_t padBytes[3] = {0, 0, 0};
    for (int y = img->height - 1; y >= 0; y--) {
        fwrite(&img->data[y * img->width], sizeof(Pixel), img->width, out);
        fwrite(padBytes, 1, padding, out);
    }

    fclose(out);
    return 1;
}

// ==========================================
// 4. IMAGE MANIPULATION FUNCTIONS
// ==========================================

void applyGrayscale(Image *img) {
    for (int i = 0; i < img->width * img->height; i++) {
        Pixel p = img->data[i];
        uint8_t gray = (uint8_t)(0.299 * p.red + 0.587 * p.green + 0.114 * p.blue);
        img->data[i].red = gray;
        img->data[i].green = gray;
        img->data[i].blue = gray;
    }
}

void applyBrightness(Image *img, int adjustment) {
    for (int i = 0; i < img->width * img->height; i++) {
        img->data[i].red = clamp(img->data[i].red + adjustment);
        img->data[i].green = clamp(img->data[i].green + adjustment);
        img->data[i].blue = clamp(img->data[i].blue + adjustment);
    }
}

Image* rotate90(Image *img) {
    Image *newImg = createImage(img->height, img->width);
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int newX = img->height - 1 - y;
            int newY = x;
            newImg->data[newY * newImg->width + newX] = img->data[y * img->width + x];
        }
    }
    return newImg;
}

Image* crop(Image *img, int startX, int startY, int cropW, int cropH) {
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (startX + cropW > img->width) cropW = img->width - startX;
    if (startY + cropH > img->height) cropH = img->height - startY;

    Image *newImg = createImage(cropW, cropH);
    for (int y = 0; y < cropH; y++) {
        for (int x = 0; x < cropW; x++) {
            newImg->data[y * cropW + x] = img->data[(startY + y) * img->width + (startX + x)];
        }
    }
    return newImg;
}

Image* applyBlur(Image *img, int scale) {
    if (scale <= 0) return copyImage(img);
    Image *src = copyImage(img);
    Image *dst = copyImage(img);

    for (int step = 0; step < scale; step++) {
        for (int y = 1; y < src->height - 1; y++) {
            for (int x = 1; x < src->width - 1; x++) {
                int sumR = 0, sumG = 0, sumB = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        Pixel p = src->data[(y + dy) * src->width + (x + dx)];
                        sumR += p.red; sumG += p.green; sumB += p.blue;
                    }
                }
                int idx = y * src->width + x;
                dst->data[idx].red = sumR / 9;
                dst->data[idx].green = sumG / 9;
                dst->data[idx].blue = sumB / 9;
            }
        }
        if (step < scale - 1) {
            memcpy(src->data, dst->data, src->width * src->height * sizeof(Pixel));
        }
    }
    freeImage(src);
    return dst; 
}

Image* applySharpen(Image *img, int scale) {
    if (scale <= 0) return copyImage(img);
    Image *src = copyImage(img);
    Image *dst = copyImage(img);
    int kernel[3][3] = { { 0, -1,  0}, {-1,  5, -1}, { 0, -1,  0} };

    for (int step = 0; step < scale; step++) {
        for (int y = 1; y < src->height - 1; y++) {
            for (int x = 1; x < src->width - 1; x++) {
                int sumR = 0, sumG = 0, sumB = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        Pixel p = src->data[(y + dy) * src->width + (x + dx)];
                        int k = kernel[dy + 1][dx + 1];
                        sumR += p.red * k; sumG += p.green * k; sumB += p.blue * k;
                    }
                }
                int idx = y * src->width + x;
                dst->data[idx].red = clamp(sumR);
                dst->data[idx].green = clamp(sumG);
                dst->data[idx].blue = clamp(sumB);
            }
        }
        if (step < scale - 1) {
            memcpy(src->data, dst->data, src->width * src->height * sizeof(Pixel));
        }
    }
    freeImage(src);
    return dst;
}

// ==========================================
// 5. IUP GUI APPLICATION
// ==========================================

// Global state
Image *currentImage = NULL;
Ihandle *image_label = NULL;

// Helper function to draw the 'currentImage' onto the IUP Label
void refresh_display() {
    if (!currentImage) return;

    int total_pixels = currentImage->width * currentImage->height;
    unsigned char *rgb_data = malloc(total_pixels * 3);
    
    int idx = 0;
    for (int i = 0; i < total_pixels; i++) {
        rgb_data[idx++] = currentImage->data[i].red;
        rgb_data[idx++] = currentImage->data[i].green;
        rgb_data[idx++] = currentImage->data[i].blue;
    }

    // Create the IUP image
    Ihandle *iup_img = IupImageRGB(currentImage->width, currentImage->height, rgb_data);
    free(rgb_data); 
    
    if (!iup_img) return;

    // Get the old image handle so we can free it later
    Ihandle *old_img = (Ihandle*)IupGetAttributeHandle(image_label, "IMAGE");

    // Directly bind the new image to the label
    IupSetAttributeHandle(image_label, "IMAGE", iup_img);
    IupSetAttribute(image_label, "TITLE", ""); 
    IupSetAttribute(image_label, "RASTERSIZE", NULL); 
    IupSetAttribute(image_label, "SIZE", NULL); 

    // Safely destroy the old image ONLY after the label has the new one
    if (old_img) {
        IupDestroy(old_img);
    }

    // Force layout update
    IupRefresh(IupGetDialog(image_label)); 
}


// --- Callbacks for Buttons ---

int btn_open_cb(Ihandle *self) {
    Ihandle *dlg = IupFileDlg();
    IupSetAttribute(dlg, "DIALOGTYPE", "OPEN");
    IupSetAttribute(dlg, "EXTFILTER", "BMP Files|*.bmp|");
    IupPopup(dlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(dlg, "STATUS") != -1) {
        char *filename = IupGetAttribute(dlg, "VALUE");
        Image *newImg = loadBMP(filename);
        if (newImg) {
            if (currentImage) freeImage(currentImage);
            currentImage = newImg;
            refresh_display();
        } else {
            IupMessage("Error", "Unsupported file or failed to open. Must be 24-bit uncompressed BMP.");
        }
    }
    IupDestroy(dlg);
    return IUP_DEFAULT;
}

int btn_save_cb(Ihandle *self) {
    if (!currentImage) return IUP_DEFAULT;

    Ihandle *dlg = IupFileDlg();
    IupSetAttribute(dlg, "DIALOGTYPE", "SAVE");
    IupSetAttribute(dlg, "EXTFILTER", "BMP Files|*.bmp|");
    IupPopup(dlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(dlg, "STATUS") != -1) {
        char *filename = IupGetAttribute(dlg, "VALUE");
        if (saveBMP(filename, currentImage)) {
            IupMessage("Success", "Image saved successfully!");
        } else {
            IupMessage("Error", "Failed to save image.");
        }
    }
    IupDestroy(dlg);
    return IUP_DEFAULT;
}

int btn_grayscale_cb(Ihandle *self) {
    if (currentImage) {
        applyGrayscale(currentImage);
        refresh_display();
    }
    return IUP_DEFAULT;
}

int btn_brightness_cb(Ihandle *self) {
    if (!currentImage) return IUP_DEFAULT;
    int adj = 0;
    if (IupGetParam("Brightness Adjustment", NULL, 0, "Amount (-255 to 255): %i\n", &adj)) {
        applyBrightness(currentImage, adj);
        refresh_display();
    }
    return IUP_DEFAULT;
}

int btn_blur_cb(Ihandle *self) {
    if (!currentImage) return IUP_DEFAULT;
    int intensity = 1;
    if (IupGetParam("Apply Blur", NULL, 0, "Intensity scale (e.g., 1 to 5): %i\n", &intensity)) {
        Image *newImg = applyBlur(currentImage, intensity);
        freeImage(currentImage);
        currentImage = newImg;
        refresh_display();
    }
    return IUP_DEFAULT;
}

int btn_sharpen_cb(Ihandle *self) {
    if (!currentImage) return IUP_DEFAULT;
    int intensity = 1;
    if (IupGetParam("Apply Sharpen", NULL, 0, "Intensity scale (e.g., 1 to 3): %i\n", &intensity)) {
        Image *newImg = applySharpen(currentImage, intensity);
        freeImage(currentImage);
        currentImage = newImg;
        refresh_display();
    }
    return IUP_DEFAULT;
}

int btn_rotate_cb(Ihandle *self) {
    if (currentImage) {
        Image *newImg = rotate90(currentImage);
        freeImage(currentImage);
        currentImage = newImg;
        refresh_display();
    }
    return IUP_DEFAULT;
}

int btn_crop_cb(Ihandle *self) {
    if (!currentImage) return IUP_DEFAULT;
    int x = 0, y = 0, w = 100, h = 100;
    if (IupGetParam("Crop Image", NULL, 0, 
                    "Start X: %i\nStart Y: %i\nWidth: %i\nHeight: %i\n", 
                    &x, &y, &w, &h)) {
        Image *newImg = crop(currentImage, x, y, w, h);
        freeImage(currentImage);
        currentImage = newImg;
        refresh_display();
    }
    return IUP_DEFAULT;
}


// --- Main Entry Point ---

int main(int argc, char **argv) {
    IupOpen(&argc, &argv);

    // Create buttons and link them to their callbacks
    Ihandle *btn_open = IupButton("Open Image", NULL);
    IupSetCallback(btn_open, "ACTION", (Icallback)btn_open_cb);

    Ihandle *btn_save = IupButton("Save Image", NULL);
    IupSetCallback(btn_save, "ACTION", (Icallback)btn_save_cb);

    Ihandle *btn_bright = IupButton("Brightness...", NULL);
    IupSetCallback(btn_bright, "ACTION", (Icallback)btn_brightness_cb);

    Ihandle *btn_gray = IupButton("Grayscale", NULL);
    IupSetCallback(btn_gray, "ACTION", (Icallback)btn_grayscale_cb);

    Ihandle *btn_blur = IupButton("Blur...", NULL);
    IupSetCallback(btn_blur, "ACTION", (Icallback)btn_blur_cb);

    Ihandle *btn_sharp = IupButton("Sharpen...", NULL);
    IupSetCallback(btn_sharp, "ACTION", (Icallback)btn_sharpen_cb);
    
    Ihandle *btn_rot = IupButton("Rotate 90", NULL);
    IupSetCallback(btn_rot, "ACTION", (Icallback)btn_rotate_cb);
    
    Ihandle *btn_crop = IupButton("Crop...", NULL);
    IupSetCallback(btn_crop, "ACTION", (Icallback)btn_crop_cb);

    // Arrange buttons in a vertical column
    Ihandle *vbox_menu = IupVbox(
        btn_open, btn_save, IupLabel("--- Filters ---"),
        btn_bright, btn_gray, btn_blur, btn_sharp, btn_rot, btn_crop,
        NULL);
    IupSetAttribute(vbox_menu, "GAP", "10");
    IupSetAttribute(vbox_menu, "MARGIN", "10x10");

    // Image display label inside a scrolling container
    image_label = IupLabel("No Image Loaded");
    IupSetAttribute(image_label, "ALIGNMENT", "ACENTER:ACENTER");
    Ihandle *scroll_box = IupScrollBox(image_label);

    // Arrange the menu on the left, and the image scrollbox on the right
    Ihandle *main_layout = IupHbox(vbox_menu, scroll_box, NULL);

    // Create the main window Dialog
    Ihandle *main_dlg = IupDialog(main_layout);
    IupSetAttribute(main_dlg, "TITLE", "BMP Image Editor");
    IupSetAttribute(main_dlg, "SIZE", "HALFxHALF"); // Start taking up half the screen

    IupShowXY(main_dlg, IUP_CENTER, IUP_CENTER);

    // Start listening for button clicks
    IupMainLoop();

    // Clean up when the window is closed
    if (currentImage) freeImage(currentImage);
    IupClose();
    return 0;
}