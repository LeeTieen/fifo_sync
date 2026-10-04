from pathlib import Path
import importlib.util
skill=Path(r'C:\Users\Admin\.codex\plugins\cache\openai-primary-runtime\documents\26.909.12148\skills\documents\render_docx.py')
spec=importlib.util.spec_from_file_location('renderer',skill)
module=importlib.util.module_from_spec(spec);spec.loader.exec_module(module)
pdf=Path('tmp/phan3/BaoCao_QA.pdf').resolve()
# Windows: Word ExportAsFixedFormat da tao PDF tu dung file DOCX moi nhat.
# Dung rasterizer chuan cua skill, chi thay buoc chuyen DOCX->PDF thieu LibreOffice.
module.convert_to_pdf=lambda *args,**kwargs:(str(pdf),'Microsoft Word ExportAsFixedFormat')
paths=module.rasterize(str(Path('output/ThucHanh_Phan3/BaoCao_ThucHanh_Phan3.docx').resolve()),
                       str(Path('tmp/phan3/render').resolve()),120,False,False)
print('Rendered',len(paths),'pages')
