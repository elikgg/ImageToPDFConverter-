import os
from tkinter import Tk, filedialog, messagebox, Button
from PIL import Image
from fpdf import FPDF


class PdfMaker:
    def __init__(self, root):
        self.root = root
        self.root.title("PDF Maker")
        self.images = []  # List to store selected images

        # Buttons
        Button(root, text="Select Images", command=self.select_images).pack(pady=10)
        Button(root, text="Create PDF", command=self.create_pdf).pack(pady=10)

    def select_images(self):
        files = filedialog.askopenfilenames(filetypes=[("Image Files", "*.jpg *.jpeg *.png *.bmp *.gif")])
        if not files:
            return

        for file in files:
            try:
                img = Image.open(file)
                self.images.append(img)
            except Exception as e:
                messagebox.showerror("Error", f"An error occurred while loading the image: {e}")

        messagebox.showinfo("Success", "Images have been added!")

    def create_pdf(self):
        if not self.images:
            messagebox.showerror("Error", "Please select at least one image.")
            return

        pdf_path = filedialog.asksaveasfilename(defaultextension=".pdf", filetypes=[("PDF Files", "*.pdf")])
        if not pdf_path:
            return

        pdf = FPDF(unit="pt", format="A4")
        try:
            for img in self.images:
                pdf.add_page()
                img_width, img_height = img.size
                pdf_width, pdf_height = 595.28, 841.89  # A4 size in points (pt)

                scale_factor = min(pdf_width / img_width, pdf_height / img_height)
                scaled_width = img_width * scale_factor
                scaled_height = img_height * scale_factor
                x = (pdf_width - scaled_width) / 2
                y = (pdf_height - scaled_height) / 2

                img_path = "temp_image.jpg"
                img.save(img_path)
                pdf.image(img_path, x=x, y=y, w=scaled_width, h=scaled_height)
                os.remove(img_path)

            pdf.output(pdf_path)
            messagebox.showinfo("Success", "PDF successfully created!")
        except Exception as e:
            messagebox.showerror("Error", f"An error occurred while creating the PDF: {e}")


if __name__ == "__main__":
    root = Tk()
    root.geometry("300x150")
    app = PdfMaker(root)
    root.mainloop()
