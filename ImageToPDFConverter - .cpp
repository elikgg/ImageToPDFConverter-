#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QPainter>
#include <QPdfWriter>

class PdfMaker : public QWidget
{
    Q_OBJECT

public:
    PdfMaker(QWidget *parent = nullptr) : QWidget(parent)
    {
        // Button to select images
        QPushButton *selectButton = new QPushButton("Select Images", this);
        selectButton->setGeometry(50, 50, 200, 40);
        connect(selectButton, &QPushButton::clicked, this, &PdfMaker::selectImages);

        // Button to create PDF
        QPushButton *createButton = new QPushButton("Create PDF", this);
        createButton->setGeometry(50, 100, 200, 40);
        connect(createButton, &QPushButton::clicked, this, &PdfMaker::createPdf);
    }

private:
    QStringList imagePaths; // List to store paths of selected images

    void selectImages()
    {
        imagePaths = QFileDialog::getOpenFileNames(this, "Select Images", QString(), "Image Files (*.jpg *.jpeg *.png *.bmp *.gif)");
        if (imagePaths.isEmpty())
        {
            QMessageBox::warning(this, "Error", "No images selected.");
        }
        else
        {
            QMessageBox::information(this, "Success", "Images have been added!");
        }
    }

    void createPdf()
    {
        if (imagePaths.isEmpty())
        {
            QMessageBox::warning(this, "Error", "Please select at least one image.");
            return;
        }

        QString pdfPath = QFileDialog::getSaveFileName(this, "Create PDF", QString(), "PDF Files (*.pdf)");
        if (pdfPath.isEmpty())
            return;

        QPdfWriter writer(pdfPath);
        writer.setPageSize(QPagedPaintDevice::A4);
        QPainter painter(&writer);

        try
        {
            for (const QString &imagePath : imagePaths)
            {
                QImage image(imagePath);
                if (image.isNull())
                {
                    throw std::runtime_error("Failed to load image.");
                }

                QRect pageRect = writer.pageRect();
                double scaleFactor = qMin(double(pageRect.width()) / image.width(), double(pageRect.height()) / image.height());
                int scaledWidth = image.width() * scaleFactor;
                int scaledHeight = image.height() * scaleFactor;
                int x = (pageRect.width() - scaledWidth) / 2;
                int y = (pageRect.height() - scaledHeight) / 2;

                painter.drawImage(QRect(x, y, scaledWidth, scaledHeight), image);
                writer.newPage();
            }

            QMessageBox::information(this, "Success", "PDF successfully created!");
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(this, "Error", e.what());
        }

        painter.end();
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PdfMaker window;
    window.resize(300, 200);
    window.setWindowTitle("PDF Maker");
    window.show();

    return app.exec();
}
