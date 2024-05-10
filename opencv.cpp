#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;

class Image
{
public:
	Image(const char* filepath, cv::ImreadModes readmode = cv::IMREAD_GRAYSCALE);
	Image(const Image& other);
	Image(const Image&& other) noexcept;
	Image& operator=(const Image& other);
	Image& operator=(Image&& other) noexcept;
	~Image();

	uchar* operator[](int r);
	uint8_t at(int r, int c) const;
	void set(int r, int c, uchar value);

	int get_rows() const;
	int get_cols() const;

	void show(std::string window_name = "") const;
private:
	cv::Mat img;
};
Image::Image(const char* filepath, cv::ImreadModes readmode)
	:img(cv::imread(filepath, readmode)) {}
Image::Image(const Image& other) :img(other.img.clone()) {}
Image::Image(const Image&& other) noexcept
	:img(std::move(other.img)) {}
Image& Image::operator=(const Image& other) 
{
	if (this != &other) 
	{
		img = other.img.clone();
	}
	return *this;
}
Image& Image::operator=(Image&& other) noexcept 
{
	img = std::move(other.img);
	return *this;
}
Image::~Image()
{
	img.release();
}
uint8_t Image::at(int r, int c) const 
{
	return static_cast<uint8_t>(img.at<uchar>(r, c));
}
void Image::set(int r, int c, uchar value)
{
	img.at<uchar>(r, c) = value;
}
uchar* Image::operator[](int r) 
{
	return img.ptr<uchar>(r);
}
int Image::get_rows() const 
{
	return img.rows;
}
int Image::get_cols() const 
{
	return img.cols;
}
void Image::show(std::string window_name) const
{
	window_name = (window_name == "" ? std::to_string((size_t)this) : window_name);
	cv::namedWindow(window_name, cv::WINDOW_NORMAL);
	cv::imshow(window_name, this->img);
	cv::waitKey(0);
}
int main1() //grayscale 
{
	Image my_image("ab.png");
	my_image.show();
	return 0;
}

int main2()  //uncropped
{
    // Your image path with proper escaping
    std::string path = "D:/oop/charliecv/charliecv/ab.png";
    cv::Mat img = cv::imread(path);

    if (img.empty()) {
        std::cerr << "Error: Image not loaded!" << std::endl;
        return -1;
    }

    int rows = img.rows;
    int cols = img.cols;

    // Allocate memory for image array with correct data type
    uchar** imgArray = new uchar * [rows];
    for (int i = 0; i < rows; i++) {
        imgArray[i] = new uchar[cols];
    }

    // Copy pixel values directly to imgArray (no need for conversion)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            imgArray[i][j] = img.at<uchar>(i, j);
        }
    }

    // Perform binarization on the image
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            imgArray[i][j] = (imgArray[i][j] >= 128) ? 255 : 0;
        }
    }

    // Create a new Mat variable for holding grayscale image
    cv::Mat processedImage(rows, cols, CV_8UC1);

    // Copy the processed image to the new Mat variable
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            processedImage.at<uchar>(i, j) = imgArray[i][j];
        }
    }

    // Deallocate the memory allocated by imgArray
    for (int i = 0; i < rows; i++) {
        delete[] imgArray[i];
    }
    delete[] imgArray;

    // Display the original and processed images
    cv::imshow("Original Image", img);
    cv::imshow("Processed Image", processedImage);

    // Resize windows to fit the screen
    cv::resizeWindow("Original Image", img.cols, img.rows);
    cv::resizeWindow("Processed Image", processedImage.cols, processedImage.rows);

    cv::waitKey(0);

    return 0;
}

int main3()  //binarize
{
    // Your image path with proper escaping
    std::string path = "D:/oop/charliecv/charliecv/ab.png";
    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);

    if (img.empty()) {
        std::cerr << "Error: Image not loaded!" << std::endl;
        return -1;
    }

    int rows = img.rows;
    int cols = img.cols;

    // Allocate memory for integer array
    int** imgArray = new int* [rows];
    for (int i = 0; i < rows; i++) {
        imgArray[i] = new int[cols];
    }

    // Convert Mat variable to integer array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            imgArray[i][j] = static_cast<int>(img.at<uchar>(i, j));
        }
    }

    // Perform binarization on the image
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            imgArray[i][j] = (imgArray[i][j] >= 128) ? 255 : 0;
        }
    }

    // Convert integer array back to Mat variable
    cv::Mat processedImage(rows, cols, CV_8UC1);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            processedImage.at<uchar>(i, j) = static_cast<uchar>(imgArray[i][j]);
        }
    }

    // Deallocate the memory allocated by imgArray
    for (int i = 0; i < rows; i++) {
        delete[] imgArray[i];
    }
    delete[] imgArray;

    // Display the original and processed images
    cv::imshow("Original Image", img);
    cv::imshow("Processed Image", processedImage);

    // Resize windows to fit the screen
    cv::resizeWindow("Original Image", img.cols, img.rows);
    cv::resizeWindow("Processed Image", processedImage.cols, processedImage.rows);

    cv::waitKey(0);

    return 0;
}

int main4()  //cropped
{
    // Your image path with proper escaping
    std::string path = "D:/oop/charliecv/charliecv/ab.png";
    cv::Mat img = cv::imread(path);

    if (img.empty()) {
        std::cerr << "Error: Image not loaded!" << std::endl;
        return -1;
    }

    // Define the coordinates and size for cropping
    int x = 100;  // starting x coordinate
    int y = 50;   // starting y coordinate
    int width = 200;  // width of the cropped region
    int height = 150; // height of the cropped region

    // Ensure the cropping region is within the image bounds
    if (x < 0 || y < 0 || x + width > img.cols || y + height > img.rows)
    {
        std::cerr << "Error: Invalid cropping region!" << std::endl;
        return -1;
    }

    // Crop the image
    cv::Rect roi(x, y, width, height);
    cv::Mat croppedImage = img(roi).clone();

    // Display the original and cropped images
    cv::imshow("Original Image", img);
    cv::imshow("Cropped Image", croppedImage);

    // Resize windows to fit the screen
    cv::resizeWindow("Original Image", img.cols, img.rows);
    cv::resizeWindow("Cropped Image", croppedImage.cols, croppedImage.rows);

    cv::waitKey(0);

    return 0;
}

int main5() //general
{
    // Your image path with proper escaping
    std::string path = "D:/oop/charliecv/charliecv/ab.png";
    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);

    if (img.empty())
    {
        std::cerr << "Error: Image not loaded!" << std::endl;
        return -1;
    }

    // Specify the desired number of generic values (levels)
    int numLevels = 4;  // Adjust this value based on requirement

    // Quantize the image to the specified number of levels
    cv::Mat quantizedImage;
    cv::normalize(img, quantizedImage, 0, numLevels - 1, cv::NORM_MINMAX, CV_8U);

    // Display the original and quantized images
    cv::imshow("Original Image", img);
    cv::imshow("Quantized Image", quantizedImage);

    // Resize windows to fit the screen
    cv::resizeWindow("Original Image", img.cols, img.rows);
    cv::resizeWindow("Quantized Image", quantizedImage.cols, quantizedImage.rows);

    cv::waitKey(0);

    return 0;
}

int main6() //rotated 
{
    // Your image path with proper escaping
    std::string path = "D:/oop/charliecv/charliecv/ab.png";
    cv::Mat img = cv::imread(path);

    if (img.empty()) {
        std::cerr << "Error: Image not loaded!" << std::endl;
        return -1;
    }

    // Rotate the image by 90 degrees clockwise
    cv::Mat rotated90;
    cv::rotate(img, rotated90, cv::ROTATE_90_CLOCKWISE);

    // Rotate the image by 180 degrees
    cv::Mat rotated180;
    cv::rotate(img, rotated180, cv::ROTATE_180);

    // Display the original and rotated images
    cv::imshow("Original Image", img);
    cv::imshow("Rotated 90 Degrees", rotated90);
    cv::imshow("Rotated 180 Degrees", rotated180);

    // Resize windows to fit the screen
    cv::resizeWindow("Original Image", img.cols, img.rows);
    cv::resizeWindow("Rotated 90 Degrees", rotated90.cols, rotated90.rows);
    cv::resizeWindow("Rotated 180 Degrees", rotated180.cols, rotated180.rows);

    cv::waitKey(0);

    return 0;

}

void menu()
    {
        cout << "1. Grayscale" << endl;
        cout << "2. Uncropped" << endl;
        cout << "3. Binarize" << endl;
        cout << "4. Cropped" << endl;
        cout << "5. General" << endl;
        cout << "6. Rotated" << endl;
        cout << "0. Exit" << endl;

    }
int main()
    {
        int choice;
        while (true)
        {
            cout << "__________________________\n" << endl;
            menu();
            cout << "__________________________\n" << endl;
            cout << "Enter Choice:";
            cin >> choice;

            switch (choice)
            {
            case 0:
                return 0;
                break;
            case 1:
                main1();

                break;
            case 2:
                main2();
                break;
            case 3:
                main3();
                break;
            case 4:
                main4();
                break;
            case 5:
                main5();
                break;

            case 6:
                main6();
                break;
            default:
                break;
            }
        }
        return 0;
    }
