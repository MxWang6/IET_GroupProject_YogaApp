namespace YogaApp
{
    partial class CategoryList
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button3 = new System.Windows.Forms.Button();
            this.CatPose = new System.Windows.Forms.Button();
            this.TreePose = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button3);
            this.groupBox1.Controls.Add(this.CatPose);
            this.groupBox1.Controls.Add(this.TreePose);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(760, 498);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Advanced Level Poses";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(273, 458);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(160, 34);
            this.button3.TabIndex = 2;
            this.button3.Text = "Go Back";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // CatPose
            // 
            this.CatPose.Location = new System.Drawing.Point(273, 77);
            this.CatPose.Name = "CatPose";
            this.CatPose.Size = new System.Drawing.Size(160, 34);
            this.CatPose.TabIndex = 1;
            this.CatPose.Text = "Cat Pose";
            this.CatPose.UseVisualStyleBackColor = true;
            this.CatPose.Click += new System.EventHandler(this.button2_Click);
            // 
            // TreePose
            // 
            this.TreePose.Location = new System.Drawing.Point(273, 19);
            this.TreePose.Name = "TreePose";
            this.TreePose.Size = new System.Drawing.Size(160, 34);
            this.TreePose.TabIndex = 0;
            this.TreePose.Text = "Tree Pose";
            this.TreePose.UseVisualStyleBackColor = true;
            this.TreePose.Click += new System.EventHandler(this.buttonTree_Click);
            // 
            // CategoryList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 518);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Name = "CategoryList";
            this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
            this.Text = "Yoga App";
            this.Load += new System.EventHandler(this.CategoryList_Load);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button CatPose;
        private System.Windows.Forms.Button button3;
        public System.Windows.Forms.Button TreePose;
    }
}